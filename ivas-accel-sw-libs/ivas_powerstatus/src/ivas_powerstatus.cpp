#include <stdio.h>
#include <stdint.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <thread>
#include <math.h>
#include <iostream>
extern "C"
{
#include <ivas/ivas_kernel.h>
#include <gst/ivas/gstinferencemeta.h>
}
#include "ivas_powerstatus.hpp"
#include "../../cm_package/cmpk_json_utils.hpp"


float PowerChart::sampleFuc(void *usrptr){
  ivas_xoverlaypriv *kpriv = (ivas_xoverlaypriv *)usrptr;
  LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");
  //毫瓦转换成瓦
  return (float)cmpk_getSomPowerInfo(kpriv->hwmonid)/1000;
}


long long
get_time ()
{
  struct timeval tv;
  gettimeofday (&tv, NULL);
  return ((long long) tv.tv_sec * 1000000 + tv.tv_usec) +
      42 * 60 * 60 * INT64_C (1000000);
}



extern "C"
{

int32_t xlnx_kernel_init (IVASKernel *handle)
{       
    json_t *jconfig = handle->kernel_config;
        // 读取所有需要处理的类
    json_t *classes = NULL,*karray = NULL;

    ivas_xoverlaypriv *kpriv = (ivas_xoverlaypriv *) calloc (1, sizeof (ivas_xoverlaypriv));
    //初始化
    kpriv->log_level = LOG_LEVEL_DEBUG;
    kpriv->font_size = 0.5;
    kpriv->font = 0;
    kpriv->line_thickness = 1;
    kpriv->y_offset = 0;
    kpriv->label_color = {0, 0, 0};
    strcpy(kpriv->label_filter[0], "class");
    strcpy(kpriv->label_filter[1], "probability");
    
    kpriv->label_filter_cnt = 2;
    kpriv->classes_count = 0;
    handle->kernel_priv = (void *) kpriv;

    // 防止没有初始化
    kpriv->powerchart = new PowerChart();

    JsonGet_int(jconfig,&(kpriv->log_level),"debug_level",0,3);
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");
    JsonGet_int(jconfig,&(kpriv->debug_param),"debug_param",10,kpriv->log_level);
    

    JsonGet_int(jconfig,&(kpriv->hwmonid),"hwmon_id",1,kpriv->log_level);

   
    uint blue,green,red;
    // chart  基础设置
    //限定叠加图像区域
    // JsonGet_bool(jconfig,&(kpriv->powerchart->enable_chart_overlay),"enable_chart_overlay",false,kpriv->log_level);
    JsonGet_int(jconfig,&(kpriv->powerchart->chart_width),"chart_width",512,kpriv->log_level);
    JsonGet_int(jconfig,&(kpriv->powerchart->chart_height),"chart_height",256,kpriv->log_level);
    JsonGet_int(jconfig,&(kpriv->powerchart->chart_abs_y),"chart_y",200,kpriv->log_level);
    JsonGet_int(jconfig,&(kpriv->powerchart->chart_abs_x),"chart_x",100,kpriv->log_level);

    // 标题文字设置
    // JsonGet_bool(jconfig,&(kpriv->powerchart->enable_txt_overlay),"enable_info_overlay",false,kpriv->log_level);
    // 自定义标题
    JsonGet_string(jconfig,&(kpriv->powerchart->title_txt),"title","SOM Power (W):",kpriv->log_level);
    //限定文字相对背景的位置
    JsonGet_int(jconfig,&(kpriv->powerchart->title_pos_y),"info_y_offset",10,kpriv->log_level);
    JsonGet_int(jconfig,&(kpriv->powerchart->title_pos_x),"info_x_offset",10,kpriv->log_level);
    JsonGet_int(jconfig,&(kpriv->powerchart->font),"font",0,kpriv->log_level);
    JsonGet_float(jconfig,&( kpriv->powerchart->font_size),"font_size",1,kpriv->log_level);
    JsonGet_rgb(jconfig,&red,&green,&blue,"label_color",0x000000,kpriv->log_level);
    kpriv->powerchart->setLableColor(red,green,blue);

    // 数据 绘图 设置    
    JsonGet_int(jconfig,&(kpriv->powerchart->maxlen),"max_sample_points",64,kpriv->log_level);
    // 使用初始化 函数自动 切换 模式 
    int power_sample_interval_ms = 0;
    JsonGet_int(jconfig,&(power_sample_interval_ms),"sample_interval_ms",100,kpriv->log_level);
    kpriv->powerchart->setSampleIntervalTime(power_sample_interval_ms);

    JsonGet_int(jconfig,&(kpriv->powerchart->line_thickness),"line_thickness",1,kpriv->log_level);
    JsonGet_float(jconfig,&(kpriv->powerchart->show_max_val),"max_display_value",15,kpriv->log_level);
    JsonGet_float(jconfig,&(kpriv->powerchart->show_min_val),"min_display_value",3,kpriv->log_level);
    JsonGet_rgb(jconfig,&red,&green,&blue,"line_color",0x00ff00,kpriv->log_level);
    kpriv->powerchart->setLineColor(red,green,blue);


    // 退出
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "exit");
    return 0;
}

uint32_t xlnx_kernel_deinit (IVASKernel *handle)
{
    ivas_xoverlaypriv *kpriv = (ivas_xoverlaypriv *) handle->kernel_priv;

    if(kpriv->powerchart)
      delete(kpriv->powerchart);

    if (kpriv)
        free (kpriv);
    return 0;
}


int32_t xlnx_kernel_start (IVASKernel *handle, int start /*unused */,
        IVASFrame *input[MAX_NUM_OBJECT], IVASFrame *output[MAX_NUM_OBJECT])
{
    
    
    GstInferenceMeta *infer_meta = NULL;
    // 初始化 private 数据
    ivas_xoverlaypriv *kpriv = (ivas_xoverlaypriv *) handle->kernel_priv;
    // 指针解套
    struct overlayframe_info *frameinfo = &(kpriv->frameinfo);
    frameinfo->inframe = input[0];
    //指向的都是相同的东西只是为了操作方便
    IVASFrame *inframe =  frameinfo->inframe;
    IVASFrameProps props = inframe->props;
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
    char *rgbdata = (char *) inframe->vaddr[0];
    
    // 如果是 NV12处理使用
    char *lumaBuf = (char *) inframe->vaddr[0];
    char *chromaBuf = (char *) inframe->vaddr[1];

    if (frameinfo->inframe->props.fmt == IVAS_VFMT_Y_UV8_420) {
      
      frameinfo->lumaImg.create (input[0]->props.height, input[0]->props.stride,CV_8UC1);
      frameinfo->lumaImg.data = (unsigned char *) lumaBuf;
      frameinfo->chromaImg.create (input[0]->props.height / 2,input[0]->props.stride / 2, CV_16UC1);
      frameinfo->chromaImg.data = (unsigned char *) chromaBuf;
    } 

    kpriv->powerchart->update(kpriv);
    
    // kpriv->powerchart->drawChartYUVLine_unprotected(frameinfo->lumaImg,frameinfo->chromaImg,3);

    // 耗时 150 us 左右
    auto start_time_all = get_time ();
    kpriv->powerchart->drawChartYUVFill_unprotected(frameinfo->lumaImg,frameinfo->chromaImg, 1);
    LOG_MESSAGE (LOG_LEVEL_INFO, kpriv->log_level, "draw chart time:%d",get_time() - start_time_all);

    // 耗时 450 us 左右 太耗时了
    start_time_all =  get_time ();
    kpriv->powerchart->drawTitle(frameinfo->lumaImg,frameinfo->chromaImg);
    kpriv->powerchart->drawDataInfo(frameinfo->lumaImg,frameinfo->chromaImg);
    LOG_MESSAGE (LOG_LEVEL_INFO, kpriv->log_level, "draw title time:%d",get_time () - start_time_all);
    

    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "exit");
    return 0;
}


int32_t xlnx_kernel_done(IVASKernel *handle)
{
    /* dummy */
    return 0;
}

}
