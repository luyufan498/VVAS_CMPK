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
#include "ivas_postsegmentation.hpp"
#include "../../cm_package/cmpk_json_utils.hpp"

#define SCALE_FLOAT_TO_INT 8
long long
get_time ()
{
  struct timeval tv;
  gettimeofday (&tv, NULL);
  return ((long long) tv.tv_sec * 1000000 + tv.tv_usec) +
      42 * 60 * 60 * INT64_C (1000000);
}

//检测传递过来的的类的名字有没有匹配的
int
ivas_classification_is_allowed (char *cls_name, ivas_xoverlaypriv * kpriv)
{
  unsigned int idx;

  if (cls_name == NULL)
    return -1;

  for (idx = 0;
      idx < sizeof (kpriv->class_list) / sizeof (kpriv->class_list[0]); idx++) {
    if (!strcmp (cls_name, kpriv->class_list[idx].class_name)) {
      return idx;
    }
  }
  return -1;
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



/* Compose label text based on config json */
bool
get_label_text (GstInferenceClassification * c, ivas_xoverlaypriv * kpriv,
    char *label_string)
{
  unsigned char idx = 0, buffIdx = 0;
  if (!c->class_label || !strlen ((char *) c->class_label))
    return false;

  for (idx = 0; idx < kpriv->label_filter_cnt; idx++) {
    if (!strcmp (kpriv->label_filter[idx], "class")) {
      sprintf (label_string + buffIdx, "%s", (char *) c->class_label);
      buffIdx += strlen (label_string);
    } else if (!strcmp (kpriv->label_filter[idx], "probability")) {
      sprintf (label_string + buffIdx, " : %.2f ", c->class_prob);
      buffIdx += strlen (label_string);
    }
  }
  return true;
}




int drawOverlaySegmentationMapYUV(ivas_xoverlaypriv *kpriv, Mat segResult)
{
    if(kpriv->frameinfo.en_overlay == false)
    {
      return 0;
    }
    struct overlayframe_info *frameinfo = &(kpriv->frameinfo);
    if(frameinfo->inframe->props.fmt != IVAS_VFMT_Y_UV8_420)
    {
        LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level, "Not IVAS_VFMT_Y_UV8_420");
        return -1;
    }

    return 0;
}


int drawOverlaySegmentationC1MapYUV(ivas_xoverlaypriv *kpriv, Mat segResult)
{
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");
    
    if(kpriv->frameinfo.en_overlay == false)
    {
      return 0;
    }

    struct overlayframe_info *frameinfo = &(kpriv->frameinfo);

    if(segResult.empty()){
        LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level, "empty seg img");
        return -1;
    }


    if(frameinfo->inframe->props.fmt != IVAS_VFMT_Y_UV8_420)
    {
        LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level, "not IVAS_VFMT_Y_UV8_420");
        return -1;
    }

    
    if(segResult.channels() != 1){
        LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level, "too many channels %d",segResult.channels());
        return -1;
    }

    int result_cnt[20]={0};



    int chromaImgy,chromaImgx;
 
    // 原始帧数据 查询位置
    // frameinfo->inframe->props.height
    // frameinfo->inframe->props.width
    // frameinfo->inframe->props.stride

    int x,y, step = 2;

    int yscale,xscale;
    
    
    xscale =  segResult.cols / frameinfo->width_overlay;
    yscale =  segResult.rows / frameinfo->height_overlay;

    //浮点转换成整数计算
    uint xscale_int = (uint)(xscale<<SCALE_FLOAT_TO_INT);
    uint yscale_int = (uint)(yscale<<SCALE_FLOAT_TO_INT);


    // 加锁：防止线程冲突
    // 循环视频的时候可能发生线程冲突，原因未知
    // 只有部分视频出现该现象
    // 似乎视频结尾的时候会发生连续多个帧堆积情况
    // 导致多个线程来读，最终卡死
    // 注意此处多线程被迫变成了单线程处理，将来可能考虑其他方案来优化
    std::lock_guard<std::mutex> lock(kpriv->mtx_);

    // for(x=0;x<segResult.cols;x=x+step){
    //   for(y=0;y<segResult.rows;y=y+step)

    for(x=0;x<frameinfo->width_overlay;x=x+2){
      for(y=0;y<frameinfo->height_overlay;y=y+2)
      {
        
        uint x_converted = (uint)x*xscale_int>>SCALE_FLOAT_TO_INT;
        uint y_converted = (uint)y*xscale_int>>SCALE_FLOAT_TO_INT;

        int val =  segResult.at<uchar>(y_converted,x_converted);

        result_cnt[val] ++;

        if( kpriv->class_list[val].class_id < 0)
          continue;
        // chromaImgy = (int) y * yscale;
        // chromaImgx = (int) x * xscale ;

        // chromaImgy = y/2 + kpriv->overlayframe_info.y_offset;
        // chromaImgx = x/2 + kpriv->overlayframe_info.x_offset;

        int lumaImgy = y + frameinfo->y_offset;
        int lumaImgx = x + frameinfo->x_offset;

        frameinfo->lumaImg.at<unsigned char>(lumaImgy, lumaImgx) = 
                            kpriv->class_list[val].converted_color.y;

        if(x%2==0 &&  y%2==0){
            frameinfo->chromaImg.at<unsigned short>(lumaImgy/2, lumaImgx/2) = 
                            kpriv->class_list[val].converted_color.uv;
        
        }

      }
    }
    // LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "Final w h :[%d,%d]",frameinfo->inframe->props.width,frameinfo->inframe->props.height);
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "Final xscale yscale :[%f,%f]",xscale,yscale);
    // LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "Final x y :[%d,%d]",chromaImgx,chromaImgy);

  
    for(auto i=0;i<20;i++)
        LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "Res %d Cnt %d",i,result_cnt[i]);

    return 0;
}


int drawOverlaySegmentationMapBGR(ivas_xoverlaypriv *kpriv, Mat segResult)
{
    if(kpriv->frameinfo.en_overlay == false)
    {
      return 0;
    }
    struct overlayframe_info *frameinfo = &(kpriv->frameinfo);
    if(frameinfo->inframe->props.fmt != IVAS_VFMT_BGR8)
    {
        LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level, "Not IVAS_VFMT_BGR8");
        return -1;
    }
    cv::Mat img_c3;
    cv::resize(segResult, img_c3, cv::Size(frameinfo->inframe->props.width,  frameinfo->inframe->props.height ), 0, 0, cv::INTER_NEAREST);
    cv::addWeighted(img_c3,0.6,frameinfo->image,0.4,0,frameinfo->image);
    return 0;
}



int drawOverylayTextInfoYUV(ivas_xoverlaypriv *kpriv, char*label_string){

    if(kpriv->scenarioinfo.en_overlay == false)
    {
      return 0;
    }

    struct overlayframe_info *frameinfo = &(kpriv->frameinfo);
    unsigned char yScalar;
    unsigned short uvScalar;
    convert_rgb_to_yuv_clrs (kpriv->label_color, &yScalar, &uvScalar);
    

    int segovelayheight = 0;
    putText (frameinfo->lumaImg, label_string, cv::Point (frameinfo->x_offset + kpriv->scenarioinfo.x_offset,
                segovelayheight + frameinfo->y_offset + kpriv->scenarioinfo.y_offset), kpriv->font, kpriv->font_size,
            Scalar (yScalar), 1, 1);
    putText (frameinfo->chromaImg, label_string, cv::Point ((frameinfo->x_offset + kpriv->scenarioinfo.x_offset)/ 2,
                segovelayheight / 2 + frameinfo->y_offset / 2 + kpriv->scenarioinfo.y_offset/2), kpriv->font,
            kpriv->font_size / 2, Scalar (uvScalar), 1, 1);


    
    return 0;
}


// 根据识别到的情况来判断场景
int classifyScenariosfromSegC1(ivas_xoverlaypriv *kpriv,Mat segResult){
      
    int result_cnt[30] = {0};
    for(auto x=0;x<segResult.cols;x++){
      for(auto y=0;y<segResult.rows;y++)
      {
        
        if(segResult.at<uchar>(y,x)<25)
        {
          result_cnt[segResult.at<uchar>(y,x)] += 1;
        }
      }
    }

    //0 road 1 sidewalk 2 building 3 wall 4 fence
    //5 pole 6 traffic light 7 traffic sign 8 vegetation 9 terrain 
    //10 sky 11 person 12 rider 13 car 14 truck 
    //15 bus 16 train 17 motorcycle 18 bicycle 19 background


    int sumpixls = segResult.cols * segResult.rows;
    int car_related = result_cnt[13] + result_cnt[14]+ result_cnt[15] +result_cnt[17];
    int people_related = result_cnt[18] + result_cnt[11] + result_cnt[12];

    char buff[40] ={0};
    int ratio = kpriv->debug_param;
    if(car_related < sumpixls/ratio && people_related < sumpixls/ratio)
    {
        if(result_cnt[0]> sumpixls/5)
        {
          LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "road: [%d]",result_cnt[0]);
          sprintf(buff, "road: [%d]",result_cnt[0]);
          drawOverylayTextInfoYUV(kpriv,buff);
          return 2;
        }
        else
        {
          LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "unknow R: [%d]",result_cnt[0]);
          sprintf(buff, "unknow R: [%d]",result_cnt[0]);
          drawOverylayTextInfoYUV(kpriv,buff);
          return -1;
        }

    }

    if(car_related > people_related * 1.5)
    {
      LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "car scenario: [%d,%d]",car_related,people_related);
      sprintf(buff, "car scenario: [%d,%d]",car_related,people_related);
      drawOverylayTextInfoYUV(kpriv,buff);
      return 0;
    }
    else
    {
      LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "people scenario: [%d,%d]",car_related,people_related);
      sprintf(buff, "people scenario: [%d,%d]",car_related,people_related);
      drawOverylayTextInfoYUV(kpriv,buff);
      return 1;
    }       


    return 0; 

}



int fifoComReportNB_segResult(cmpk::fifocom *ffc,int result,const char *info)
{
    char buff[255] = {0};
    sprintf(buff,"reportSeg,%f,%s\n",result,info);


    // return cmpk::ivas_fifocommuncation_send_raw(*ffc,buff,strlen(buff));
    return cmpk::fifoComWriteNB_string(ffc,buff);
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
    kpriv->frameinfo.lastSegImg.release();
    kpriv->scenarioinfo.y_offset = 10;
    kpriv->scenarioinfo.x_offset = 10;

    for(auto i=0;i<MAX_ALLOWED_CLASS;i++)
      kpriv->class_list[i].class_id = -1;

    XkprivGetJsonData_int(jconfig,&(kpriv->log_level),"debug_level",0,3);
    XkprivGetJsonData_int(jconfig,&(kpriv->debug_param),"debug_param",10,kpriv->log_level);
    
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");

    XkprivGetJsonData_int(jconfig,&(kpriv->frameinfo.y_offset),"y_offset_abs",200,kpriv->log_level);
    XkprivGetJsonData_int(jconfig,&(kpriv->frameinfo.x_offset),"x_offset_abs",100,kpriv->log_level);

    XkprivGetJsonData_int(jconfig,&(kpriv->frameinfo.width_overlay),"overlay_width",512,kpriv->log_level);
    XkprivGetJsonData_int(jconfig,&(kpriv->frameinfo.height_overlay),"overlay_height",256,kpriv->log_level);

    XkprivGetJsonData_bool(jconfig,&(kpriv->scenarioinfo.en_overlay),"enable_info_overlay",false,kpriv->log_level);
    XkprivGetJsonData_bool(jconfig,&(kpriv->frameinfo.en_overlay),"enable_frame_overlay",false,kpriv->log_level);

    XkprivGetJsonData_int(jconfig,&(kpriv->scenarioinfo.y_offset),"info_y_offset",-10,kpriv->log_level);
    XkprivGetJsonData_int(jconfig,&(kpriv->scenarioinfo.x_offset),"info_x_offset",10,kpriv->log_level);

    int font_size;
    if(XkprivGetJsonData_int(jconfig,&(font_size),"font_size",1,kpriv->log_level)){
      kpriv->font_size = font_size;
    }
    // XkprivGetJsonData_int(jconfig,&(kpriv->font_size),"font_size",1,kpriv->log_level);

    XkprivGetJsonData_uint(jconfig,&(kpriv->font),"font",0,kpriv->log_level);
    XkprivGetJsonData_int(jconfig,&(kpriv->line_thickness),"thickness",1,kpriv->log_level);

    int baseline;
    Size textsize;
    textsize = getTextSize ("A", kpriv->font, kpriv->font_size, 1, &baseline);
    kpriv->scenarioinfo.y_offset = 10 + textsize.height ;


    karray = json_object_get (jconfig, "label_color");
    if (!karray) {
      LOG_MESSAGE (LOG_LEVEL_ERROR,kpriv->log_level, "failed to find label_color");
    }
    else{
      XkprivGetJsonData_uint(karray,&(kpriv->label_color.blue),"blue",0,kpriv->log_level);
      XkprivGetJsonData_uint(karray,&(kpriv->label_color.green),"green",0,kpriv->log_level);
      XkprivGetJsonData_uint(karray,&(kpriv->label_color.red),"red",0,kpriv->log_level);
    }


    karray = json_object_get (jconfig, "classes");
    if (!karray) {
      LOG_MESSAGE (LOG_LEVEL_ERROR,kpriv->log_level, "failed to find classes labels");
      return -1;
    }
    if (!json_is_array (karray)) {
      LOG_MESSAGE (LOG_LEVEL_ERROR,kpriv->log_level, "classes is not array type");
      return -1;
    }
    // 循环读取所有的label
    kpriv->classes_count = json_array_size (karray);
    for (unsigned int index = 0; index < kpriv->classes_count; index++) {
      classes = json_array_get (karray, index);
      if (!classes) {
        LOG_MESSAGE (LOG_LEVEL_ERROR,kpriv->log_level, "failed to get {..} object, skip #%d",index);
        break;
      }
      int classID;
      XkprivGetJsonData_int(classes,&(classID),"id",-1,kpriv->log_level);
      if(classID >= MAX_ALLOWED_CLASS){
        LOG_MESSAGE (LOG_LEVEL_WARNING,kpriv->log_level, "reach maximum classnum %d, drop others",MAX_ALLOWED_CLASS);
        break;
      }    
      kpriv->class_list[classID].class_id = classID;
      XkprivGetJsonData_uint(classes,&(kpriv->class_list[classID].class_color.green),"green",255,kpriv->log_level);
      XkprivGetJsonData_uint(classes,&(kpriv->class_list[classID].class_color.blue),"blue",255,kpriv->log_level);
      XkprivGetJsonData_uint(classes,&(kpriv->class_list[classID].class_color.red),"red",255,kpriv->log_level);
      XkprivGetJsonData_chars(classes,kpriv->class_list[classID].class_name,"name","", kpriv->log_level);
      // 提前转换到 YUV 备用
      unsigned char *yScalar =  &kpriv->class_list[classID].converted_color.y;
      unsigned short *uvScalar = &kpriv->class_list[classID].converted_color.uv;
      convert_rgb_to_yuv_clrs (kpriv->class_list[classID].class_color, yScalar, uvScalar);
     }

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
    
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");

    static bool segMapEn = false;
    // static Mat lastSegChromaImg;
    // frameinfo->lastSegChromaImg = lastSegChromaImg;

    //标记一下数据类型
    if(props.fmt== IVAS_VFMT_BGR8){
        LOG_MESSAGE (LOG_LEVEL_INFO, kpriv->log_level, "FRAME FMT: IVAS_VFMT_BGR8");
    }
    else if(props.fmt == IVAS_VFMT_Y_UV8_420)
    {
        LOG_MESSAGE (LOG_LEVEL_INFO, kpriv->log_level, "FRAME FMT: IVAS_VFMT_Y_UV8_420");
    }
    else
    {
        LOG_MESSAGE (LOG_LEVEL_INFO, kpriv->log_level, "FRAME FMT: %d",props.fmt);
    }

    // 输入的帧数据
    // 如果是 RGB 处理使用
    char *rgbdata = (char *) inframe->vaddr[0];
    
    // 如果是 NV12处理使用
    char *lumaBuf = (char *) inframe->vaddr[0];
    char *chromaBuf = (char *) inframe->vaddr[1];

    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");

    //提取infer_meta
    infer_meta = ((GstInferenceMeta *) gst_buffer_get_meta ((GstBuffer *)
            inframe->app_priv, gst_inference_meta_api_get_type ()));


    if(infer_meta == NULL)
    {
      LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "can not find infer meta");
      return 0;
    }

    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");

    //提取root predict节点
    GstInferencePrediction *root = infer_meta->prediction;

    bool notfound = true;
    //提取第一层所有的节点
    GSList *tmp = gst_inference_prediction_get_children(root);
    for (GSList *child_predictions = tmp; child_predictions; child_predictions = g_slist_next(child_predictions)){
        //child_predictions 是一个 list迭代器
        // data 存放的是 predict指针
        GstInferencePrediction *child = (GstInferencePrediction *)child_predictions->data;

        //检测 classifications 是否存放了数据
        GList *fristclass = g_list_first(child->classifications);
        if(fristclass == NULL){
          continue;
        }
        //拿出 classifications ID 用 10000 表示
        GstInferenceClassification *classification = (GstInferenceClassification *) fristclass->data;
        if(classification->class_id != 10000)
        {
          //不是 segmentation 的数据
          continue;
        }
        // 异常检测
        if(child->sub_buffer == NULL)
        {
            LOG_MESSAGE (LOG_LEVEL_WARNING, kpriv->log_level, "empty sub_buffer");
            continue;
        }
      
        //提取之前seg的结果
        GstBuffer *buffer = (GstBuffer *)child->sub_buffer; 
        //同样是内存映射
        //info.data 映射到 buffer内部
        GstMapInfo info;
        gst_buffer_map(buffer, &info, GST_MAP_READ);

        // vmeta的作用就是单纯的取得宽度 和 长度
        // video meta 可能是 gst自己内部实现的一个meta
        GstVideoMeta *vmeta = gst_buffer_get_video_meta(buffer);
        
        //转换成MAT进行操作
        char *indata = (char *)info.data;
        cv::Mat segResultImg(vmeta->height, vmeta->width, CV_8UC1, indata);
        
        //对结果进行存档
        segResultImg.copyTo(frameinfo->lastSegImg);
        segMapEn = true;
        
        LOG_MESSAGE (LOG_LEVEL_INFO, kpriv->log_level, "vmeta size info: %d %d",vmeta->height, vmeta->width);

        gst_buffer_unmap(buffer, &info);
        
        // 只处理读到的第一个 segmen 的结果
    
        break;

    }



    
    
    char *pstr;
        /* Print the entire prediction tree */
    pstr = gst_inference_prediction_to_string (infer_meta->prediction);
    LOG_MESSAGE (LOG_LEVEL_DEBUG,kpriv->log_level, "Prediction tree: \n%s", pstr);
    free (pstr);


    // 没有被赋值的时候无法 调用或者赋值给其他mat
    // 也不能被函数使用
    // 如果不加 第一次调用会GG
    // 空的mat被函数调用会直接报错
    if(frameinfo->lastSegImg.empty()){
      LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");
      return 0;  
    }
    
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");

    if(props.fmt == IVAS_VFMT_Y_UV8_420)
      {
          //并不是长宽，因为做了行对齐
          frameinfo->lumaImg.create (input[0]->props.height, input[0]->props.stride,CV_8UC1);
          frameinfo->lumaImg.data = (unsigned char *) input[0]->vaddr[0];
          frameinfo->chromaImg.create (input[0]->props.height / 2, input[0]->props.stride / 2, CV_16UC1);
          frameinfo->chromaImg.data = (unsigned char *) input[0]->vaddr[1];
          
          auto start_time = get_time ();
          drawOverlaySegmentationC1MapYUV(kpriv,frameinfo->lastSegImg);
          int segclassRes= classifyScenariosfromSegC1(kpriv,frameinfo->lastSegImg);

          fifoComReportNB_segResult(&kpriv->ffc,segclassRes,"segmentation");

          auto end_time = get_time ();
          // 打印时间     
          LOG_MESSAGE (LOG_LEVEL_INFO, kpriv->log_level, "time cost of draw Seg :%d ms",(end_time - start_time)/1000);
       
      }
      else
      if (frameinfo->inframe->props.fmt == IVAS_VFMT_BGR8) 
      {
          frameinfo->image.create (input[0]->props.height, input[0]->props.stride / 3, CV_8UC3); 
          frameinfo->image.data = (unsigned char *) input[0]->vaddr[0];  
          drawOverlaySegmentationMapBGR(kpriv,frameinfo->lastSegImg);
      }
      else
      {
          LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level, "Unsupported FMT:%d",frameinfo->inframe->props.fmt);
      }



    // if(frameinfo->lastSegChromaImg.data){     
      
    //   LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level, "Draw In");
    //   // Mat imageROI = frameinfo->chromaImg(Rect(0, 0, frameinfo->lastSegChromaImg.cols, frameinfo->lastSegChromaImg.rows)); 
    //   // cv::Rect roi_rect = cv::Rect(0, 0, frameinfo->lastSegChromaImg.cols, frameinfo->lastSegChromaImg.rows);
    //   frameinfo->lastSegChromaImg.copyTo(frameinfo->chromaImg);
    // }


  
    auto end_time_all = get_time ();
    LOG_MESSAGE (LOG_LEVEL_INFO, kpriv->log_level, "all time cost:%d",end_time_all - start_time_all);
         

    
    return 0;
}


int32_t xlnx_kernel_done(IVASKernel *handle)
{
    /* dummy */
    return 0;
}

}
