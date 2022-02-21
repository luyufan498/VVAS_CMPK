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
#include "ivas_indicater.hpp"
#include <cm_package/cmpk_json_utils.hpp>


using namespace cv;
using namespace std;

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




void drawICON(ivas_xoverlaypriv *kpriv){

    static int framecnt = 0;
    framecnt ++;

    struct overlayframe_info *frameinfo = &(kpriv->frameinfo);
    if(kpriv->status == 1)
    {
        color clr = {0,0,255};
        unsigned char yScalar;
        unsigned short uvScalar;
        convert_rgb_to_yuv_clrs (clr, &yScalar, &uvScalar);
        rectangle (frameinfo->lumaImg, Rect (kpriv->x_pos,kpriv->y_pos, kpriv->width,kpriv->width), Scalar (yScalar), FILLED, 1, 0);
        rectangle (frameinfo->chromaImg, Rect (kpriv->x_pos/2,kpriv->y_pos/2, kpriv->width/2,kpriv->width/2), Scalar (uvScalar), FILLED, 1, 0);
    }
    else
    {
        if(framecnt%(kpriv->show_on + kpriv->show_off) < kpriv->show_off)
            return;

        color clr = {0,255,0};
        unsigned char yScalar;
        unsigned short uvScalar;
        convert_rgb_to_yuv_clrs (clr, &yScalar, &uvScalar);
        int radius = kpriv->width/2;
        int x_center = kpriv->x_pos + radius;
        int y_center = kpriv->y_pos + radius;
        circle(frameinfo->lumaImg,   Point (x_center,y_center),     radius,     Scalar (yScalar), FILLED);
        circle(frameinfo->chromaImg, Point (x_center/2,y_center/2), radius/2,   Scalar (uvScalar), FILLED);
    }
}


#define FFCCMD_PARAMS 1
static int fifoComCtr_DynamicModel_reid(ivas_xoverlaypriv *kpriv){
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");
    
    cmpk::fifocom *ffc = &(kpriv->ffc);
    cmpk::fifoComRead(ffc);
    std::string header = "runindicater";
    if(ffc->lines_buffer.size()<(FFCCMD_PARAMS+1)) return false; 
    if(ffc->lines_buffer[0].compare(header)) return false;

    
    if(ffc->lines_buffer[1].compare("1"))
    {
        kpriv->status = 1;   
    }
    else
    {
        kpriv->status = 0;   
    }
    
    cout<< "Get UI ctr:"<<kpriv->status<<" buff:"<<ffc->lines_buffer[1]<<endl;
    return true;
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


    JsonGet_int(jconfig,&(kpriv->log_level),"debug_level",0,3);
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");
    JsonGet_int(jconfig,&(kpriv->debug_param),"debug_param",10,kpriv->log_level);
    JsonGet_int(jconfig,&(kpriv->status),"default_status",0,kpriv->log_level);
    JsonGet_int(jconfig,&(kpriv->x_pos),"x_pos",50,kpriv->log_level);
    JsonGet_int(jconfig,&(kpriv->y_pos),"y_pos",50,kpriv->log_level);
    JsonGet_int(jconfig,&(kpriv->width),"width",50,kpriv->log_level);
    JsonGet_int(jconfig,&(kpriv->show_on),"show_on",20,kpriv->log_level);
    JsonGet_int(jconfig,&(kpriv->show_off),"show_off",10,kpriv->log_level);

    // not in use
    JsonGet_string(jconfig,&(kpriv->ffc.txpath),"ffc_txpath","/home/petalinux/.temp/runstatus1_tx", kpriv->log_level);
    //实际应该只用到接受参数，不需要发送
    JsonGet_string(jconfig,&(kpriv->ffc.rxpath),"ffc_rxpath","/home/petalinux/.temp/runstatus1_rx", kpriv->log_level);

    // 退出
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
    // LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");

    //标记一下数据类型
    if(props.fmt== IVAS_VFMT_BGR8){
        LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "FRAME FMT: IVAS_VFMT_BGR8");
        return 0;
    }
    else if(props.fmt == IVAS_VFMT_Y_UV8_420)
    {
        // LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "FRAME FMT: IVAS_VFMT_Y_UV8_420");
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

    fifoComCtr_DynamicModel_reid(kpriv);
    drawICON(kpriv);



    // LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "exit");
    return 0;
}


int32_t xlnx_kernel_done(IVASKernel *handle)
{
    /* dummy */
    return 0;
}

}
