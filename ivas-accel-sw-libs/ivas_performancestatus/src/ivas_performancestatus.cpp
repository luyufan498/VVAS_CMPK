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
#include "ivas_performancestatus.hpp"
#include "../../cm_package/cmpk_json_utils.hpp"



long long
get_time ()
{
  struct timeval tv;
  gettimeofday (&tv, NULL);
  return ((long long) tv.tv_sec * 1000000 + tv.tv_usec) +
      42 * 60 * 60 * INT64_C (1000000);
}





//色彩转换
void
convert_rgb_to_yuv_clrs (color clr, unsigned char *y, unsigned short *uv)
{
  Mat YUVmat;
  Mat BGRmat (2, 2, CV_8UC3, Scalar (clr.red, clr.green, clr.blue));
  cvtColor (BGRmat, YUVmat, cv::COLOR_BGR2YUV_I420);
  *y = YUVmat.at < uchar > (0, 0);
  *uv = YUVmat.at < uchar > (2, 0) << 8 | YUVmat.at < uchar > (2, 1);
  return;
}


// 把值转换成y相对坐标
int convertVal2Yoffset(ivas_xoverlaypriv *kpriv, long val)
{
  if(val > kpriv->sampleinfo.maxval)
  {
    return 0;
  }

  return (kpriv->frameinfo.height -  (kpriv->sampleinfo.y_ratio * val));
}


// 把数据点存放到里面
int pushbackPoint(ivas_xoverlaypriv *kpriv){
  sample_info *sampleinfo = &(kpriv->sampleinfo);
  LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");
 
  // 转换成帧来操作
  static int frame_interval = sampleinfo->sample_interval_ms / 30;
  static int frame_cnt = 0;
  frame_cnt ++ ;

  if(frame_interval && frame_cnt%frame_interval !=0)
  {
    return -1;
  }

  // static auto lasttime = get_time ();
  // if( get_time () -lasttime <  sampleinfo->sample_interval_ms*1000)
  // {
  //   // LOG_MESSAGE (LOG_LEVEL_DEBUG,kpriv->log_level, "time duration:%ld",get_time () -lasttime);
  //   return -1;
  // }
  // lasttime = get_time ();

  
  //多线程保护
  std::lock_guard<std::mutex> lock(kpriv->mtx_);

  data_point pp;
  static int readcnt = 0;
  // pp.data = cmpk_getPlatformTempInfo(2, sampleinfo->deviceid);
  pp.data = kpriv->pf.avgFPS;
  pp.y_converted = convertVal2Yoffset(kpriv,pp.data);
  LOG_MESSAGE (LOG_LEVEL_DEBUG,kpriv->log_level, "avgFPS:%ld, Converted y:%d, read_cnt: %d",pp.data,pp.y_converted,readcnt++);
 
  //读取power数据
  sampleinfo->points.push_back(pp);
  //检查长度
  if(sampleinfo->points.size()> sampleinfo->maxlen){
    vector<data_point>::iterator k = sampleinfo->points.begin();
    sampleinfo->points.erase(k);
  }
  return 0;
}

int updateRefreshXoffset(ivas_xoverlaypriv *kpriv)
{
  sample_info *sampleinfo = &(kpriv->sampleinfo);
  LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");
 
  // static auto lasttime = get_time ();
  // auto interval =  (get_time () -lasttime)/1000 ;

  // if( interval <  sampleinfo->refresh_interval_ms)
  // {
  //   // LOG_MESSAGE (LOG_LEVEL_DEBUG,kpriv->log_level, "time duration:%ld",get_time () -lasttime);
  //   return -1;
  // }
  // lasttime = get_time ();
  
  // 转换成帧来操作
  static int frame_interval = sampleinfo->sample_interval_ms / 30;
  static int frame_cnt = 0;
  frame_cnt ++;
  
  if(sampleinfo->maxlen > sampleinfo->points.size())
  {
    return 0;
  }


  int offset = 30.0 / sampleinfo->sample_interval_ms * sampleinfo->x_interval * (frame_cnt%frame_interval); 
  sampleinfo->refresh_x_offset = offset;
  // sampleinfo->refresh_x_offset %= sampleinfo->x_interval;

  LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "offset x ADD %d = [%d]",offset,sampleinfo->refresh_x_offset);

  return 0;

}

// 此处 x y 的值都是相对位置
int getPixlColorYUV(ivas_xoverlaypriv *kpriv, int x, int y ,unsigned char *yScalar,unsigned short *uvScalar)
{
  // LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");
  sample_info *sampleinfo =  &(kpriv->sampleinfo);
  struct overlayframe_info *frameinfo = &(kpriv->frameinfo);

  int x_calibrated = x + sampleinfo->refresh_x_offset;
  // 找 X 对应的 数据点 位置
  int pos = (x_calibrated )/sampleinfo->x_interval;
  int x_offset = (x_calibrated)%sampleinfo->x_interval;
    

  // 检查数据是否溢出
  if(pos <  ((int)sampleinfo->points.size() - 1))
  {
    
    // LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter1 %d  %d",pos,sampleinfo->points.size());
    // 检查y 是否大于 （在下面）
    int new_y =  (sampleinfo->points[pos + 1].y_converted - sampleinfo->points[pos].y_converted) 
    * sampleinfo->multable[x_offset] 
    + sampleinfo->points[pos].y_converted;

    // LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter2");
    if(y > new_y)
    {
      // LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter x=%d,y=%d [%d]",x,y,pos);
      *yScalar = (sampleinfo->line_color_yuv.y);
      *uvScalar = (sampleinfo->line_color_yuv.uv);

    }
  }
  return 0;
}

int getDrawOffset(ivas_xoverlaypriv *kpriv, int x, int *pos, int *y_calibrated)
{
  // LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");
  sample_info *sampleinfo =  &(kpriv->sampleinfo);
  struct overlayframe_info *frameinfo = &(kpriv->frameinfo);

  int x_calibrated = x + sampleinfo->refresh_x_offset;
  // 找 X 对应的 数据点 位置
  *pos = (x_calibrated )/sampleinfo->x_interval;
  int x_offset = (x_calibrated)%sampleinfo->x_interval;
    

  // 检查数据是否溢出
  if(*pos <  ((int)sampleinfo->points.size() - 1))
  {
    // LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter1 %d  %d",pos,sampleinfo->points.size());
    // 检查y 是否大于 （在下面）
    int new_y =  (sampleinfo->points[*pos + 1].y_converted - sampleinfo->points[*pos].y_converted) 
    * sampleinfo->multable[x_offset] 
    + sampleinfo->points[*pos].y_converted;

    // LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter2");
    *y_calibrated=new_y;
    return true;

  }
  else
  {
    *pos = -1;
    return false;
    
  }
}

int drawOverlayYUV(ivas_xoverlaypriv *kpriv)
{
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");
    
    if(kpriv->frameinfo.en_overlay == false)
    {
      return 0;
    }
    struct overlayframe_info *frameinfo = &(kpriv->frameinfo);
    struct sample_info *sampleinfo =  &(kpriv->sampleinfo);

    if(frameinfo->inframe->props.fmt != IVAS_VFMT_Y_UV8_420)
    {
        LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level, "not IVAS_VFMT_Y_UV8_420");
        return -1;
    }

    unsigned char back_yScalar, yScalar;
    unsigned short back_uvScalar, uvScalar;
    convert_rgb_to_yuv_clrs (frameinfo->back_color, &back_yScalar, &back_uvScalar);

    std::lock_guard<std::mutex> lock(kpriv->mtx_);

    int chromaImgy,chromaImgx;

    int pos,y_calibrated;

    // for(int i = 0;i<sampleinfo->points.size();i++)
    // {
    //   LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "%d,%ld,%d",i,sampleinfo->points[i].data,sampleinfo->points[i].y_converted);
    // }

    // 画出背景
    for(auto x=0;x<frameinfo->width;x=x+2){
      //先确认x点位对应的pos位置数据
      getDrawOffset(kpriv,x,&pos,&y_calibrated);

      for(auto y=0;y<frameinfo->height;y=y+2)
      {
        //跳过不需要的值
        if(y<y_calibrated || pos < 0)
        {
          yScalar = back_yScalar;
          uvScalar = back_uvScalar;
        }
        else
        {
          yScalar = (sampleinfo->line_color_yuv.y);
          uvScalar = (sampleinfo->line_color_yuv.uv); 
        }

        int lumaImgy = y + frameinfo->y_offset;
        int lumaImgx = x + frameinfo->x_offset;



        //检查颜色是否是需要的颜色
        // getPixlColorYUV(kpriv,x,y,&yScalar,&uvScalar);



        frameinfo->lumaImg.at<unsigned char>(lumaImgy, lumaImgx) = 
                           yScalar;

        if(lumaImgy%2==0 &&  lumaImgx%2==0){
            frameinfo->chromaImg.at<unsigned short>(lumaImgy/2, lumaImgx/2) = 
                            uvScalar; 
        }

      }
    }

    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "exit");
    return 0;
}


int drawOverlayYUV_v2(ivas_xoverlaypriv *kpriv)
{
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");
    
    if(kpriv->frameinfo.en_overlay == false)
    {
      return 0;
    }
    struct overlayframe_info *frameinfo = &(kpriv->frameinfo);
    struct sample_info *sampleinfo =  &(kpriv->sampleinfo);

    if(frameinfo->inframe->props.fmt != IVAS_VFMT_Y_UV8_420)
    {
        LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level, "not IVAS_VFMT_Y_UV8_420");
        return -1;
    }

    unsigned char back_yScalar, yScalar;
    unsigned short back_uvScalar, uvScalar;
    convert_rgb_to_yuv_clrs (frameinfo->back_color, &back_yScalar, &back_uvScalar);

    std::lock_guard<std::mutex> lock(kpriv->mtx_);

    int chromaImgy,chromaImgx;

    int pos,y_calibrated;

    // for(int i = 0;i<sampleinfo->points.size();i++)
    // {
    //   LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "%d,%ld,%d",i,sampleinfo->points[i].data,sampleinfo->points[i].y_converted);
    // }

    // 画出背景
    for(auto x=0;x<frameinfo->width;x=x+2){
      //先确认x点位对应的pos位置数据
      getDrawOffset(kpriv,x,&pos,&y_calibrated);

      for(auto y=0;y<frameinfo->height;y=y+2)
      {
        //跳过不需要的值
        if(pos < 0 || y<y_calibrated-1 || y>y_calibrated)
        {
          if(kpriv->frameinfo.en_back){
            yScalar = back_yScalar;
            uvScalar = back_uvScalar;
          }else
          {
            continue;
          }
        }
        else
        {
          yScalar = (sampleinfo->line_color_yuv.y);
          uvScalar = (sampleinfo->line_color_yuv.uv); 
        }

        int lumaImgy = y + frameinfo->y_offset;
        int lumaImgx = x + frameinfo->x_offset;



        //检查颜色是否是需要的颜色
        // getPixlColorYUV(kpriv,x,y,&yScalar,&uvScalar);



        frameinfo->lumaImg.at<unsigned char>(lumaImgy, lumaImgx) = 
                           yScalar;

        if(lumaImgy%2==0 &&  lumaImgx%2==0){
            frameinfo->chromaImg.at<unsigned short>(lumaImgy/2, lumaImgx/2) = 
                            uvScalar; 
        }

      }
    }

    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "exit");
    return 0;
}


int drawOverylayTextInfoYUV(ivas_xoverlaypriv *kpriv, char*label_string, int x_rel, int y_rel){

    if(kpriv->textinfo.en_overlay == false)
    {
      return 0;
    }

    // LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");
    // int baseline;
    // Size textsize;
    // textsize = getTextSize (label_string, kpriv->font, kpriv->font_size, 1, &baseline);
    struct overlayframe_info *frameinfo = &(kpriv->frameinfo);
    int x = x_rel + frameinfo->x_offset;
    int y = y_rel + frameinfo->y_offset;

  
    unsigned char yScalar;
    unsigned short uvScalar;
    convert_rgb_to_yuv_clrs (kpriv->label_color, &yScalar, &uvScalar);
    
    putText (frameinfo->lumaImg, label_string, cv::Point (x,y), kpriv->font, kpriv->font_size,Scalar (yScalar), 1, 1);
    putText (frameinfo->chromaImg, label_string, cv::Point ((x)/ 2,y/2), kpriv->font,kpriv->font_size / 2, Scalar (uvScalar), 1, 1);

    // LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "exit");
    return 0;
}

int drawOverylayStrLineYUV(ivas_xoverlaypriv *kpriv, char*label_string, int line_num)
{
   float y_rel = 1.2 * (line_num + 1) * kpriv->textinfo.line_height;
   drawOverylayTextInfoYUV(kpriv,label_string,10,(int)y_rel);
   return 0;
}

int drawMaxMinPowerInfo(ivas_xoverlaypriv *kpriv){
  
  sample_info *sampleinfo =  &(kpriv->sampleinfo);
  int len = sampleinfo->points.size();
  
  if(len < 1)
  {
    return 0;
  }

  auto datamax = sampleinfo->points[0].data;
  auto datamin = sampleinfo->points[0].data;
  auto datasum = sampleinfo->points[0].data;

  for(auto i = 1;i<len;i++)
  { 
    if(sampleinfo->points[i].data > datamax)
    {
      datamax = sampleinfo->points[i].data;
    }

    if(sampleinfo->points[i].data < datamin)
    {
      datamin = sampleinfo->points[i].data;
    }

    datasum +=  sampleinfo->points[i].data;

  }

  char title[1][20] = {
    "FPS:"
  };

  // LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "p info title1");
  drawOverylayStrLineYUV(kpriv,title[0],0);
  // LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "p info title2");
  char buff[60]={0};
  sprintf(buff,"Max:%3.2f Min:%3.2f Avg:%3.2f",(float)datamax,(float)datamin,(float)kpriv->pf.avgFPS);
  // LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "p info :%s",buff);
  drawOverylayStrLineYUV(kpriv,buff,1);
  return 0;

}

int
performanceTestRecord(ivas_xoverlaypriv * kpriv)
{
  ivas_perf *pf = &kpriv->pf;
  // 记录帧数据
  // 无论怎么样都记录帧号
  pf->frames++;
  static int windowlen = 60;
  // 存60帧的数据
    //多线程保护
  std::lock_guard<std::mutex> lock(kpriv->mtx_);
  
  pf->displayed_time_list.insert(pf->displayed_time_list.begin(),get_time());
  if(pf->displayed_time_list.size()> 90){
    pf->displayed_time_list.erase(pf->displayed_time_list.end());
  }

  if(pf->displayed_time_list.size() > windowlen)
  {
     long long current_time = pf->displayed_time_list[0];
     pf->last_displayed_time = pf->displayed_time_list[windowlen];
     double time = (current_time - pf->last_displayed_time) / 1000000.0;
     double fps = (time > 0.0) ? ((float)(windowlen) / time) : 999.99;
     pf->avgFPS = fps;

    if (kpriv->performance_test &&  pf->frames % kpriv->ffc.tx_frame_interval == 0) { 




        // 传递数据出去
        auto res = cmpk::fifoComReportNB_fps(&kpriv->ffc,fps,"performance");
        // cmpk::ivas_fifocommuncation_send_raw(kpriv->ffc, buff,strlen(buff));

        LOG_MESSAGE (LOG_LEVEL_INFO, kpriv->log_level, "FFC results: %d",res);

        // printf ("\rframe=%5lu fps=%6.*f        \r", pf->frames,(fps < 9.995) ? 3 : 2, fps); fflush (stdout);
    }
  
  
  }


  // if (get_time () - pf->last_displayed_time >= 1000000.0) {
  //   long long current_time = get_time ();
  //   double time = (current_time - pf->last_displayed_time) / 1000000.0;
  //   pf->last_displayed_time = current_time;
  //   double fps = (time > 0.0) ? ((pf->frames - pf->last_displayed_frame) / time) : 999.99;
  //   pf->last_displayed_frame = pf->frames;
    
  //   pf->avgFPS = fps;



  // }
  return 0;
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
 
    kpriv->textinfo.line_height = 0;
    kpriv->textinfo.line_index = 0;

    kpriv->frameinfo.back_color ={100,100,100};

    kpriv->sampleinfo.line_color = {0,0,255};
    kpriv->sampleinfo.maxlen = 32;
 
    kpriv->sampleinfo.sample_interval_ms= 200;
    kpriv->sampleinfo.refresh_x_offset= 0;
    kpriv->sampleinfo.refresh_interval_ms= 30;
    kpriv->sampleinfo.deviceid =  get_device_hwmon_id(0,"ams");

    kpriv->sampleinfo.maxval= 15000;

    for(auto i=0;i<MAX_ALLOWED_CLASS;i++)
      kpriv->class_list[i].class_id = -1;

    XkprivGetJsonData_int(jconfig,&(kpriv->log_level),"debug_level",0,3);
    XkprivGetJsonData_int(jconfig,&(kpriv->debug_param),"debug_param",10,kpriv->log_level);
    
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");

    XkprivGetJsonData_int(jconfig,&(kpriv->sampleinfo.targetid),"target_id",2,kpriv->log_level);
    if(kpriv->sampleinfo.targetid>2 or kpriv->sampleinfo.targetid<0)
    {
      kpriv->sampleinfo.targetid = 2;
    }



    //管道地址
    XkprivGetJsonData_bool(jconfig,&(kpriv->performance_test),"enable_fifocom",false,kpriv->log_level);
    XkprivGetJsonData_string(jconfig,&(kpriv->ffc.txpath),"ffc_tx","/home/petalinux/.temp/pf_tx", kpriv->log_level);
    XkprivGetJsonData_string(jconfig,&(kpriv->ffc.rxpath),"ffc_rx","/home/petalinux/.temp/pf_rx", kpriv->log_level);
    XkprivGetJsonData_int(jconfig,&(kpriv->ffc.tx_frame_interval),"ffc_tx_interval",10, kpriv->log_level);


    //限定叠加图像区域
    XkprivGetJsonData_bool(jconfig,&(kpriv->frameinfo.en_overlay),"enable_frame_overlay",false,kpriv->log_level);
    XkprivGetJsonData_bool(jconfig,&(kpriv->frameinfo.en_back),"enable_background",false,kpriv->log_level);
    XkprivGetJsonData_int(jconfig,&(kpriv->frameinfo.width),"overlay_width",512,kpriv->log_level);
    XkprivGetJsonData_int(jconfig,&(kpriv->frameinfo.height),"overlay_height",256,kpriv->log_level);
    XkprivGetJsonData_int(jconfig,&(kpriv->frameinfo.y_offset),"y_offset_abs",200,kpriv->log_level);
    XkprivGetJsonData_int(jconfig,&(kpriv->frameinfo.x_offset),"x_offset_abs",100,kpriv->log_level);

    //限定文字相对背景的位置
    XkprivGetJsonData_bool(jconfig,&(kpriv->textinfo.en_overlay),"enable_info_overlay",false,kpriv->log_level);
    XkprivGetJsonData_int(jconfig,&(kpriv->textinfo.y_offset),"info_y_offset",-10,kpriv->log_level);
    XkprivGetJsonData_int(jconfig,&(kpriv->textinfo.x_offset),"info_x_offset",10,kpriv->log_level);

    //采样与波形图设置
    XkprivGetJsonData_int(jconfig,&(kpriv->sampleinfo.sample_interval_ms),"sample_interval_ms",100,kpriv->log_level);
    XkprivGetJsonData_int(jconfig,&(kpriv->sampleinfo.refresh_interval_ms),"refresh_interval_ms",30,kpriv->log_level);
    XkprivGetJsonData_int(jconfig,&(kpriv->sampleinfo.maxlen),"max_sample_points",64,kpriv->log_level);
    XkprivGetJsonData_int(jconfig,&(kpriv->sampleinfo.maxval),"max_display_val",80000,kpriv->log_level);

    int font_size;
    if(XkprivGetJsonData_int(jconfig,&(font_size),"font_size",1,kpriv->log_level)){
      kpriv->font_size = font_size;
    }


    int baseline;
    Size textsize;
    textsize = getTextSize ("A", kpriv->font, kpriv->font_size, 1, &baseline);
    // 计算出行高度
    // 作为打印log的参考
    kpriv->textinfo.line_height = textsize.height;
    // XkprivGetJsonData_int(jconfig,&(kpriv->font_size),"font_size",1,kpriv->log_level);

    XkprivGetJsonData_uint(jconfig,&(kpriv->font),"font",0,kpriv->log_level);
    XkprivGetJsonData_int(jconfig,&(kpriv->line_thickness),"thickness",1,kpriv->log_level);

    karray = json_object_get (jconfig, "label_color");
    if (!karray) {
      LOG_MESSAGE (LOG_LEVEL_ERROR,kpriv->log_level, "failed to find label_color");
    }
    else{
      XkprivGetJsonData_uint(karray,&(kpriv->label_color.blue),"blue",0,kpriv->log_level);
      XkprivGetJsonData_uint(karray,&(kpriv->label_color.green),"green",0,kpriv->log_level);
      XkprivGetJsonData_uint(karray,&(kpriv->label_color.red),"red",0,kpriv->log_level);
    }



    karray = json_object_get (jconfig, "line_color");
    if (!karray) {
      LOG_MESSAGE (LOG_LEVEL_ERROR,kpriv->log_level, "failed to find label_color");
    }
    else{
      XkprivGetJsonData_uint(karray,&(kpriv->sampleinfo.line_color.blue),"blue",0,kpriv->log_level);
      XkprivGetJsonData_uint(karray,&(kpriv->sampleinfo.line_color.green),"green",0,kpriv->log_level);
      XkprivGetJsonData_uint(karray,&(kpriv->sampleinfo.line_color.red),"red",0,kpriv->log_level);
    }



    // 计算每个点的间隔
    kpriv->sampleinfo.x_interval = (kpriv->frameinfo.width ) /  (kpriv->sampleinfo.maxlen-2);
    // 计算值和y坐标的比例
    kpriv->sampleinfo.y_ratio = 1.0 * (kpriv->frameinfo.height ) / kpriv->sampleinfo.maxval;



    // 提前计算比例
    for(auto i=0;i<MULTABLELEN;i++){
      
      if(i==0)
        kpriv->sampleinfo.multable[i] = 0;
      else
      if(i%2==1)
        kpriv->sampleinfo.multable[i] = kpriv->sampleinfo.multable[i-1];
      else
        kpriv->sampleinfo.multable[i] = (float)i/ kpriv->sampleinfo.x_interval;

      LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "multable[%d]:%f",i,kpriv->sampleinfo.multable[i]);
    }




    LOG_MESSAGE (LOG_LEVEL_DEBUG,kpriv->log_level, "y ratio %f",kpriv->sampleinfo.y_ratio);
    convert_rgb_to_yuv_clrs (kpriv->sampleinfo.line_color, &(kpriv->sampleinfo.line_color_yuv.y), &(kpriv->sampleinfo.line_color_yuv.uv));

    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "exit");
    return 0;
}

uint32_t xlnx_kernel_deinit (IVASKernel *handle)
{
    ivas_xoverlaypriv *kpriv = (ivas_xoverlaypriv *) handle->kernel_priv;
    if (kpriv)
        free (kpriv);
    return 0;
}


int32_t xlnx_kernel_start (IVASKernel *handle, int start /*unused */,
        IVASFrame *input[MAX_NUM_OBJECT], IVASFrame *output[MAX_NUM_OBJECT])
{
    
    auto start_time_all = get_time ();
    GstInferenceMeta *infer_meta = NULL;
    // 初始化 private 数据
    ivas_xoverlaypriv *kpriv = (ivas_xoverlaypriv *) handle->kernel_priv;
    // 指针解套
    struct overlayframe_info *frameinfo = &(kpriv->frameinfo);
    frameinfo->inframe = input[0];
    //指向的都是相同的东西只是为了操作方便
    IVASFrame *inframe =  frameinfo->inframe;
    IVASFrameProps props = inframe->props;
    struct sample_info *sampleinfo =  &(kpriv->sampleinfo);
    
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");


    //标记一下数据类型
    if(props.fmt== IVAS_VFMT_BGR8){
        LOG_MESSAGE (LOG_LEVEL_INFO, kpriv->log_level, "FRAME FMT: IVAS_VFMT_BGR8");
        return 0;
    }
    else if(props.fmt == IVAS_VFMT_Y_UV8_420)
    {
        LOG_MESSAGE (LOG_LEVEL_INFO, kpriv->log_level, "FRAME FMT: IVAS_VFMT_Y_UV8_420");
    }
    else
    {
        LOG_MESSAGE (LOG_LEVEL_INFO, kpriv->log_level, "FRAME FMT: %d",props.fmt);
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

    
    performanceTestRecord(kpriv);
    //更新数据点
    pushbackPoint(kpriv);
    updateRefreshXoffset(kpriv);

    //画图
    drawOverlayYUV_v2(kpriv);

    //画信息
    drawMaxMinPowerInfo(kpriv);
    
    auto end_time_all = get_time ();
    LOG_MESSAGE (LOG_LEVEL_INFO, kpriv->log_level, "all time cost:%d",end_time_all - start_time_all);
         
    // for(auto i=0;i< sampleinfo->points.size();i ++ )
    // {
    //   LOG_MESSAGE (LOG_LEVEL_INFO, kpriv->log_level, "pp:[%d] %d", i, sampleinfo->points[i].y_converted);
    // }


    
    return 0;
}


int32_t xlnx_kernel_done(IVASKernel *handle)
{
    /* dummy */
    return 0;
}

}
