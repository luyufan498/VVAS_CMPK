#include <stdio.h>
#include <stdint.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <thread>
#include <math.h>
#include <iostream>
#include <fstream>
extern "C"
{
#include <ivas/ivas_kernel.h>
#include <gst/ivas/gstinferencemeta.h>
}
#include "ivas_chart.hpp"
#include <cm_package/cmpk_json_utils.hpp>


void ivas_perf::writefps2file(){
    
    ofstream outfile;
    outfile.open(this->savefilepath.c_str(), ios::out | ios::trunc);
    outfile<<this->avgFPS<<endl;
    outfile.close();
    
}



long long
get_time ()
{
  struct timeval tv;
  gettimeofday (&tv, NULL);
  return ((long long) tv.tv_sec * 1000000 + tv.tv_usec) +
      42 * 60 * 60 * INT64_C (1000000);
}

int
performanceUpdate(ivas_xoverlaypriv * kpriv)
{
  if(!kpriv->fps_en)
    return 0;
  
  ivas_perf *pf = &kpriv->perf;
  pf->frames++;
  std::lock_guard<std::mutex> lock(kpriv->mtx_);
  pf->displayed_time_list.push_back(get_time());
  if(pf->displayed_time_list.size() > pf->windowlen){
      pf->displayed_time_list.erase(pf->displayed_time_list.begin());
  }
  if(pf->displayed_time_list.size()>2){
      double time = (pf->displayed_time_list.back() - pf->displayed_time_list.front())/1000000.0;
      pf->avgFPS = time>0? (pf->displayed_time_list.size() - 1)/time:999.99; 
  }

  if(kpriv->ffc_en){
      auto res = cmpk::fifoComReportNB_fps(&kpriv->ffc,pf->avgFPS,"perf_from_sensor_pulgin");
  }


  if(kpriv->perf.iswrite2file){
      kpriv->perf.writefps2file();
  }

  return 0;
}



long getDataFromCustomSensor_long(ivas_xoverlaypriv * kpriv){
    FILE *fp;
	long data;
    fp = fopen(kpriv->sensor.path.c_str(),"r");
	if(fp == NULL)
	{
		printf("unable to open %s\n",kpriv->sensor.path.c_str());
		return -1;
	}
	fscanf(fp,"%ld",&data);
	fclose(fp);
	return (data);
}

float getDataFromCustomSensor_float(ivas_xoverlaypriv * kpriv){
    FILE *fp;
	float data;
    fp = fopen(kpriv->sensor.path.c_str(),"r");
	if(fp == NULL)
	{
		printf("unable to open %s\n",kpriv->sensor.path.c_str());
		return -1;
	}

    fseek(fp, 0L, SEEK_END);
    auto sz = ftell(fp);
    if(sz <= 0){
        fclose(fp);
        return -2;
    }
        
    fseek(fp, 0L, SEEK_SET);

	fscanf(fp,"%f",&data);
    if(data < 0){
        printf("error data:%f size:%d\n",data,sz);
        fclose(fp);
        return -3;
    }
	fclose(fp);
	return (data)*kpriv->sensor.scale;
}

float ivas_chart::sampleFuc(void *usrptr){
  ivas_xoverlaypriv *kpriv = (ivas_xoverlaypriv *)usrptr;
  switch (kpriv->mode)
  {
    case CHART_MODE_POWER:
        return (float)cmpk_getSomPowerInfo(1)/1000;
        break;
    case CHART_MODE_LPD_TEMP: 
        // 0：LPD_TEMP 1：FPD_TEMP 2：PL_TEMP 
        return (float)cmpk_getPlatformTempInfo(CHART_MODE_LPD_TEMP,0)/1000;
        break;
    case CHART_MODE_FPD_TEMP: 
      // 0：LPD_TEMP 1：FPD_TEMP 2：PL_TEMP 
        return (float)cmpk_getPlatformTempInfo(CHART_MODE_FPD_TEMP,0)/1000;
        break;
    case CHART_MODE_PL_TEMP: 
      // 0：LPD_TEMP 1：FPD_TEMP 2：PL_TEMP 
        return (float)cmpk_getPlatformTempInfo(CHART_MODE_PL_TEMP,0)/1000;
        break;

    case CHART_MODE_FPS: 
      // 0：LPD_TEMP 1：FPD_TEMP 2：PL_TEMP 
        return kpriv->perf.avgFPS;
        break;
    
    case CHART_MODE_CUSTOM_LONG: 
        return (float)getDataFromCustomSensor_long(kpriv)*kpriv->sensor.scale;
        break;
    
    case CHART_MODE_CUSTOM_FLOAT: 
        return getDataFromCustomSensor_float(kpriv)*kpriv->sensor.scale;
        break;

  default:
      return (float)cmpk_getSomPowerInfo(1)/1000;
      break;
  }
}





extern "C"
{

int32_t xlnx_kernel_init (IVASKernel *handle)
{       
    json_t *jconfig = handle->kernel_config;
        // 读取所有需要处理的类
    // json_t *classes = NULL;
    // json_t *karray = NULL;

    ivas_xoverlaypriv *kpriv = (ivas_xoverlaypriv *) calloc (1, sizeof (ivas_xoverlaypriv));
    //初始化
    kpriv->log_level = LOG_LEVEL_DEBUG;

    handle->kernel_priv = (void *) kpriv;

    // 防止没有初始化
    kpriv->cmpkchart = new ivas_chart();

    JsonGet_int(jconfig,&(kpriv->log_level),"debug_level",0,3);
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");
    JsonGet_int(jconfig,&(kpriv->debug_param),"debug_param",10,kpriv->log_level);
    

    JsonGet_int(jconfig,&(kpriv->sensor.hwmonid),"hwmon_id",1,kpriv->log_level);

    

    JsonGet_int(jconfig,&(kpriv->mode),"senor_mode",CHART_MODE_CUSTOM_LONG,kpriv->log_level);
    
    // only work in custom mode
    JsonGet_string(jconfig,&(kpriv->sensor.path),"sensor_path","/sys/class/hwmon/hwmon1/power1_input",kpriv->log_level);
    // only work in custom mode
    JsonGet_float(jconfig,&( kpriv->sensor.scale),"sensor_scale",0.001,kpriv->log_level);
    
    // fps settings
    JsonGet_bool(jconfig,&(kpriv->fps_en),"enable_fps",true,kpriv->log_level);
    JsonGet_int(jconfig,&(kpriv->perf.windowlen),"fps_window_len",30,kpriv->log_level);

    JsonGet_bool(jconfig,&(kpriv->perf.iswrite2file),"enable_fps_w2f",false,kpriv->log_level);
    JsonGet_string(jconfig,&(kpriv->perf.savefilepath),"fps_write_path","/home/petalinux/.temp/fps",kpriv->log_level);

    //管道地址
    XkprivGetJsonData_bool(jconfig,&(kpriv->ffc_en),"enable_fifocom",false,kpriv->log_level);
    XkprivGetJsonData_string(jconfig,&(kpriv->ffc.txpath),"ffc_tx","/home/petalinux/.temp/pf_tx", kpriv->log_level);
    XkprivGetJsonData_string(jconfig,&(kpriv->ffc.rxpath),"ffc_rx","/home/petalinux/.temp/pf_rx", kpriv->log_level);

    uint blue,green,red;
    // chart  基础设置
    //限定叠加图像区域
    JsonGet_bool(jconfig,&(kpriv->chart_line_en),"enable_chart_overlay",false,kpriv->log_level);
    JsonGet_int(jconfig,&(kpriv->cmpkchart->chart_width),"chart_width",512,kpriv->log_level);
    JsonGet_int(jconfig,&(kpriv->cmpkchart->chart_height),"chart_height",256,kpriv->log_level);
    JsonGet_int(jconfig,&(kpriv->cmpkchart->chart_abs_y),"chart_y",200,kpriv->log_level);
    JsonGet_int(jconfig,&(kpriv->cmpkchart->chart_abs_x),"chart_x",100,kpriv->log_level);

    
    // 标题文字设置
    JsonGet_bool(jconfig,&(kpriv->cmpkchart->title_en),"enable_info_overlay",false,kpriv->log_level);
    JsonGet_bool(jconfig,&(kpriv->cmpkchart->datainfo_en),"enable_analysis_overlay",false,kpriv->log_level);
    
    JsonGet_int(jconfig,&(kpriv->cmpkchart->line_offset),"info_line_offset",0,kpriv->log_level);
    JsonGet_int(jconfig,&(kpriv->cmpkchart->datainfo_type),"analysis_type",0,kpriv->log_level);
 
    // 自定义标题
    JsonGet_string(jconfig,&(kpriv->cmpkchart->title_txt),"title","<Title Name>:",kpriv->log_level);
    //限定文字相对背景的位置
    JsonGet_int(jconfig,&(kpriv->cmpkchart->title_pos_y),"info_y_offset",10,kpriv->log_level);
    JsonGet_int(jconfig,&(kpriv->cmpkchart->title_pos_x),"info_x_offset",10,kpriv->log_level);
    JsonGet_int(jconfig,&(kpriv->cmpkchart->font),"font",0,kpriv->log_level);
    JsonGet_float(jconfig,&( kpriv->cmpkchart->font_size),"font_size",1,kpriv->log_level);
    JsonGet_rgb(jconfig,&red,&green,&blue,"label_color",0x000000,kpriv->log_level);
    kpriv->cmpkchart->setLableColor(red,green,blue);
    kpriv->cmpkchart->setLegendColor(red,green,blue);
    if(JsonGet_rgb(jconfig,&red,&green,&blue,"legend_color",0x000000,kpriv->log_level))
    {
        kpriv->cmpkchart->setLegendColor(red,green,blue);
    }


    // 数据 绘图 设置    

    JsonGet_int(jconfig,&(kpriv->cmpkchart->maxlen),"max_sample_points",64,kpriv->log_level);
    // 使用初始化 函数自动 切换 模式 
    int power_sample_interval_ms = 0;
    JsonGet_int(jconfig,&(power_sample_interval_ms),"sample_interval_ms",100,kpriv->log_level);
    kpriv->cmpkchart->setSampleIntervalTime(power_sample_interval_ms);

    JsonGet_int(jconfig,&(kpriv->cmpkchart->chart_display_type),"chart_type",0,kpriv->log_level);
    JsonGet_int(jconfig,&(kpriv->cmpkchart->performance_optimize),"chart_perf_optimize",4,kpriv->log_level);
    JsonGet_int(jconfig,&(kpriv->cmpkchart->line_thickness),"line_thickness",1,kpriv->log_level);
    JsonGet_float(jconfig,&(kpriv->cmpkchart->show_max_val),"max_display_value",15,kpriv->log_level);
    JsonGet_float(jconfig,&(kpriv->cmpkchart->show_min_val),"min_display_value",3,kpriv->log_level);
    JsonGet_rgb(jconfig,&red,&green,&blue,"line_color",0x00ff00,kpriv->log_level);
    kpriv->cmpkchart->setLineColor(red,green,blue);


    // 退出
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "exit");
    return 0;
}

uint32_t xlnx_kernel_deinit (IVASKernel *handle)
{
    ivas_xoverlaypriv *kpriv = (ivas_xoverlaypriv *) handle->kernel_priv;

    if(kpriv->cmpkchart)
      delete(kpriv->cmpkchart);

    if (kpriv)
        free (kpriv);
    return 0;
}


int32_t xlnx_kernel_start (IVASKernel *handle, int start /*unused */,
        IVASFrame *input[MAX_NUM_OBJECT], IVASFrame *output[MAX_NUM_OBJECT])
{
    
    // 用不到
    // GstInferenceMeta *infer_meta = NULL;
    // 初始化 private 数据
    ivas_xoverlaypriv *kpriv = (ivas_xoverlaypriv *) handle->kernel_priv;
    // 指针解套
    struct overlayframe_info *frameinfo = &(kpriv->frameinfo);
    frameinfo->inframe = input[0];
    //指向的都是相同的东西只是为了操作方便
    IVASFrame *inframe =  frameinfo->inframe;
    IVASFrameProps props = inframe->props;
    cmpk::chart *cmpkchart = kpriv->cmpkchart; 


    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");


    //标记一下数据类型
    if(props.fmt== IVAS_VFMT_BGR8){
        LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "FRAME FMT: IVAS_VFMT_BGR8");
        return 0;
    }
    else if(props.fmt == IVAS_VFMT_Y_UV8_420)
    {
        LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "FRAME FMT: IVAS_VFMT_Y_UV8_420");
    }
    else
    {
        LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "FRAME FMT: %d",props.fmt);
        return 0;
    }

    // 输入的帧数据
    // 如果是 RGB 处理使用
    // char *rgbdata = (char *) inframe->vaddr[0];
    
    // 如果是 NV12处理使用
    char *lumaBuf = (char *) inframe->vaddr[0];
    char *chromaBuf = (char *) inframe->vaddr[1];

    if (frameinfo->inframe->props.fmt == IVAS_VFMT_Y_UV8_420) {
      
      frameinfo->lumaImg.create (input[0]->props.height, input[0]->props.stride,CV_8UC1);
      frameinfo->lumaImg.data = (unsigned char *) lumaBuf;
      frameinfo->chromaImg.create (input[0]->props.height / 2,input[0]->props.stride / 2, CV_16UC1);
      frameinfo->chromaImg.data = (unsigned char *) chromaBuf;
    } 

    kpriv->cmpkchart->update(kpriv);
    
    // kpriv->cmpkchart->drawChartYUVLine_unprotected(frameinfo->lumaImg,frameinfo->chromaImg,3);

    // 耗时 150 us 左右
    auto start_time_all = get_time ();
    if(kpriv->chart_line_en)
        kpriv->cmpkchart->drawChartYUV(frameinfo->lumaImg,frameinfo->chromaImg);

    auto drawcharttime = get_time() - start_time_all;

    // 耗时 450 us 左右 太耗时了
    start_time_all =  get_time ();

    kpriv->cmpkchart->drawTitle(frameinfo->lumaImg,frameinfo->chromaImg);
    kpriv->cmpkchart->drawDataInfo(frameinfo->lumaImg,frameinfo->chromaImg);

    // int lnum = 0;
    // if(kpriv->chart_title_en)
    // {
    //     kpriv->cmpkchart->drawTitle(frameinfo->lumaImg,frameinfo->chromaImg);
    //     lnum ++;
    // }
    // if(kpriv->chart_anlysis_en)
    // {
    //     char buff[60]={0};
    //     sprintf(buff,"Max:%3.2f Min:%3.2f Avg:%3.2f",cmpkchart->data_max,cmpkchart->data_min,this->data_avg);
    //     kpriv->cmpkchart->drawStr(buff,lnum,lumaImg,chromaImg)
    //     // kpriv->cmpkchart->drawDataInfo(frameinfo->lumaImg,frameinfo->chromaImg);

    // }
        

    auto drawtxttime = get_time() - start_time_all;

    LOG_MESSAGE (LOG_LEVEL_INFO, kpriv->log_level, "%s draw txt time:%lld, draw chart: %lld",kpriv->cmpkchart->title_txt.c_str(),drawtxttime,drawcharttime);
    
    

    performanceUpdate(kpriv);
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "exit");
    return 0;
}


int32_t xlnx_kernel_done(IVASKernel *handle)
{
    /* dummy */
    return 0;
}

}
