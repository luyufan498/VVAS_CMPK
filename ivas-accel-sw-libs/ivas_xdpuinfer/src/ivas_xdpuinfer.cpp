/*
 * Copyright 2020 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * file: ivas_xdpuinfer.cpp
 *
 * ivas_xdpuinfer is the dynamic library used with gstreamer ivas filter
 * plugins to have a generic interface for Xilinx DPU Library and applications.
 * ivas_xdpuinfer supports different models of DPU based on model class.
 * Any new class can be added with minimal effort.
 *
 * Example json file parameters required for ivas_xdpuinfer
 * {
 * "xclbin-location":"/usr/lib/dpu.xclbin",
 * "ivas-library-repo": "/usr/local/lib/ivas/",
 * "element-mode":"inplace",
 * "kernels" :[
 *  {
 *    "library-name":"libivas_xdpuinfer.so",
 *    "config": {
 *      "model-name" : "resnet50",
 *      "model-class" : "CLASSIFICATION",
 *      "model-path" : "/usr/share/vitis_ai_library/models/",
 *      "run_time_model" : flase,
 *      "need_preprocess" : true,
 *      "performance_test" : true,
 *      "debug_level" : 1
 *    }
 *   }
 *  ]
 * }
 *
 * Details of above parametres id under "struct ivas_xkpriv"
 *
 * Example pipe:
 * gst-launch-1.0 filesrc location="./images/001.bgr" blocksize=150528 num-buffers=1 !  \
 * videoparse width=224 height=224 framerate=30/1 format=16 ! \
 * ivas_xfilter name="kernel1" kernels-config="./json_files/kernel_resnet50.json" ! \
 * ivas_xfilter name="kernel2" kernels-config="./json_files/kernel_testresnet50.json" ! \
 * filesink location=./resnet_output_224_224.bgr
 */

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>

#include <vitis/ai/bounded_queue.hpp>
#include <vitis/ai/env_config.hpp>

extern "C"
{
#include <ivas/ivas_kernel.h>
}
#include <gst/ivas/gstinferencemeta.h>
#include <gst/ivas/gstivasinpinfer.h>


#include "ivas_xdpupriv.hpp"
#include "ivas_xdpumodels.hpp"

#include "../../cm_package/cmpk_segmentation.hpp"
#include "../../cm_package/cmpk_json_utils.hpp"

#include "dpuinfer_partial_ffc.hpp"
#include "dpuinfer_partial_model.hpp"



using namespace cv;
using namespace std;
using namespace cmpk;

ivas_xdpumodel::~ivas_xdpumodel ()
{
}

/**
 * fileexists () - Check either file exists or not
 *
 * check either able to open the file whoes path is in name
 *
 */
inline bool
fileexists (const string & name)
{
  struct stat buffer;
  return (stat (name.c_str (), &buffer) == 0);
}

// 启动Pf 记录帧
int
performanceTestStart(ivas_xkpriv * kpriv)
{
  ivas_perf *pf = &kpriv->pf;
  if (kpriv->performance_test && !kpriv->pf.test_started) {
      pf->timer_start = get_time ();
      pf->last_displayed_time = pf->timer_start;
      pf->test_started = 1;
    }
  return 0;
}

// 计算当前帧 并且进行记录
int
performanceTestRecord(ivas_xkpriv * kpriv)
{
  ivas_perf *pf = &kpriv->pf;
  // 记录帧数据
  // 无论怎么样都记录帧号
  pf->frames++;

  if(!kpriv->performance_test)
    return 0;

  if(!kpriv->pf.test_started)
    return 0;


  if (get_time () - pf->last_displayed_time >= 1000000.0) {
    long long current_time = get_time ();
    double time = (current_time - pf->last_displayed_time) / 1000000.0;
    pf->last_displayed_time = current_time;
    double fps = (time > 0.0) ? ((pf->frames - pf->last_displayed_frame) / time) : 999.99;
    pf->last_displayed_frame = pf->frames;
    
    pf->avgFPS = fps;

    if (kpriv->performance_test && kpriv->pf.test_started) {
          
        char buff[20] = {0};
        sprintf(buff,"FPS:%f \r",fps);
        // 传递数据出去
        // cmpk::ivas_fifocommuncation_send_raw(kpriv->ffc, buff,strlen(buff));
        printf ("\rframe=%5lu fps=%6.*f        \r", pf->frames,(fps < 9.995) ? 3 : 2, fps); fflush (stdout);
    }
  }



  return 0;
}




extern "C"
{

  int32_t xlnx_kernel_init (IVASKernel * handle)
  {
    ivas_xkpriv *kpriv = (ivas_xkpriv *) calloc (1, sizeof (ivas_xkpriv));
      kpriv->handle = handle;

    json_t *jconfig = handle->kernel_config;
    json_t *val,*karray = NULL,*jmodel = NULL;                /* kernel config from app */

    XkprivGetJsonData_int(jconfig,&(kpriv->log_level),"debug_level",0,3);
    XkprivGetJsonData_bool(jconfig,&(kpriv->run_time_model),"run_time_model",false, kpriv->log_level);
    XkprivGetJsonData_bool(jconfig,&(kpriv->performance_test),"performance_test",false, kpriv->log_level);
    XkprivGetJsonData_bool(jconfig,&(kpriv->need_preprocess),"need_preprocess",true, kpriv->log_level);
    XkprivGetJsonData_bool(jconfig,&(kpriv->enable),"enable",true, kpriv->log_level);
    XkprivGetJsonData_bool(jconfig,&(kpriv->buff_en),"buff_en",true, kpriv->log_level);


    // 类型是枚举，没办法直接处理
    string tmp_videofmt;
    XkprivGetJsonData_string(jconfig,&(tmp_videofmt),"model-format","BGR", kpriv->log_level);
    kpriv->modelfmt = ivas_fmt_to_xfmt (tmp_videofmt.data());
    if (kpriv->modelfmt == IVAS_VMFT_UNKNOWN) {
      LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level,"SORRY NOT SUPPORTED MODEL FORMAT %s",(char *) json_string_value (val));
      goto err;
    }

    XkprivGetJsonData_string(jconfig,&(kpriv->modelpath),"model-path","/usr/share/vitis_ai_library/models/", kpriv->log_level);
    if (!fileexists (kpriv->modelpath)) {
      LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level,
          "model-path (%s) not exist", kpriv->modelpath.c_str ());
      goto err;
    }

    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "mid");
    //自己添加的参数
    XkprivGetJsonData_string(jconfig,&(kpriv->ffc.txpath),"ffc_txpath",FIFO_WRITE, kpriv->log_level);
    XkprivGetJsonData_string(jconfig,&(kpriv->ffc.rxpath),"ffc_rxpath",FIFO_READ, kpriv->log_level);
    XkprivGetJsonData_int(jconfig,&(kpriv->ffc.tx_frame_interval),"tx_frame_interval",30,kpriv->log_level);
    XkprivGetJsonData_int(jconfig,&(kpriv->ffc.rx_frame_interval),"rx_frame_interval",30,kpriv->log_level);
    XkprivGetJsonData_int(jconfig,&(kpriv->target_fps),"target_fps",30,kpriv->log_level);
    XkprivGetJsonData_int(jconfig,&(kpriv->interval_frames),"interval_frames",1,kpriv->log_level);

   

    //由ivas_xclass_to_num 来进行内部转换  
    // typedef int (*xkprivStringProcessAPI)(char *); 
    XkprivGetJsonData_string2Int(jconfig,&(kpriv->modelclass),"model-class",ivas_xclass_to_num,IVAS_XCLASS_NOTFOUND, kpriv->log_level);
    if (kpriv->modelclass == IVAS_XCLASS_NOTFOUND) {
      LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level,"SORRY NOT SUPPORTED MODEL CLASS %s",(char *) json_string_value (val));
      goto err;
    }
    XkprivGetJsonData_string(jconfig,&(kpriv->modelname),"model-name","", kpriv->log_level);
    kpriv->elfname = modelexits (kpriv);
    if (kpriv->elfname.empty ()) {
      goto err;
    }

    XkprivGetJsonData_int(jconfig,&(kpriv->priority),"priority",0,kpriv->log_level);
    //fifo communication
//---------------------------------------------------
    
    LOG_MESSAGE (LOG_LEVEL_INFO, kpriv->log_level, "model-name = %s",
        (char *) json_string_value (val));
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "model class is %d",
        kpriv->modelclass);
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "elf class is %s",
        kpriv->elfname.c_str ());

    kpriv->model = ivas_xinitmodel (kpriv, kpriv->modelclass);
    ivas_xsetcaps(kpriv,kpriv->model);
    if (kpriv->model == NULL) {
      LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level,
          "Init ivas_xinitmodel failed for %s", kpriv->modelname.c_str ());
      goto err;
    }

    handle->kernel_priv = (void *) kpriv;
    return true;

  err:
    free (kpriv);
    return -1;
  }

  uint32_t xlnx_kernel_deinit (IVASKernel * handle)
  {
    ivas_xkpriv *kpriv = (ivas_xkpriv *) handle->kernel_priv;
    if (!kpriv)
      return true;
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");

    ivas_perf *pf = &kpriv->pf;

    if (kpriv->performance_test && kpriv->pf.test_started) {
      double time = (get_time () - pf->timer_start) / 1000000.0;
      double fps = (time > 0.0) ? (pf->frames / time) : 999.99;
      printf ("\rframe=%5lu fps=%6.*f        \n", pf->frames,
          (fps < 9.995) ? 3 : 2, fps);
    }
    pf->test_started = 0;
    pf->frames = 0;
    pf->last_displayed_frame = 0;
    pf->timer_start = 0;
    pf->last_displayed_time = 0;

    if (!kpriv->run_time_model) {
      for (int i = 0; i < int (kpriv->mlist.size ()); i++) {
        if (kpriv->mlist[i].model) {
          kpriv->mlist[i].model->close ();
          delete kpriv->mlist[i].model;
          kpriv->mlist[i].model = NULL;
        }
        kpriv->model = NULL;
      }
    }
    kpriv->modelclass = IVAS_XCLASS_NOTFOUND;

    if (kpriv->model != NULL) {
      kpriv->model->close ();
      delete kpriv->model;
      kpriv->model = NULL;
    }
    if (kpriv->labelptr != NULL)
      free (kpriv->labelptr);

    ivas_caps_free (handle);
    free (kpriv);

    return true;
  }

  uint32_t xlnx_kernel_start (IVASKernel * handle, int start,
      IVASFrame * input[MAX_NUM_OBJECT], IVASFrame * output[MAX_NUM_OBJECT])
  {
    ivas_xkpriv *kpriv = (ivas_xkpriv *) handle->kernel_priv;
    cmpk::fifocom *ffc = &kpriv->ffc;
    ivas_perf *pf = &kpriv->pf;
    GstInferenceMeta *infer_meta = NULL;
    GstIvasInpInferMeta *ivas_inputmeta = NULL;
    IVASFrame *inframe = input[0];
    char *indata = (char *) inframe->vaddr[0];
    int ret, i;

    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");
    

    // 每次都要读，确保清空缓冲区
    // cmpk::fifoComRead(ffc);
    // loadDynamicModelfromFFC(kpriv);
    //开始进行

    fifoComCtrAll(kpriv);
    // 加入关闭插件的功能
    // 打开插件的时候需要注意顺序的问题


    // 需要把infer meta的指针拿出来给 DPU存结果
    infer_meta = (GstInferenceMeta *) gst_buffer_add_meta ((GstBuffer *)inframe->app_priv, gst_inference_meta_get_info (), NULL);

    if (infer_meta == NULL) {
      LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level,"ivas meta data is not available for dpu");
      return -1;
    } 

    
    if(!kpriv->enable)
    {
      infer_meta->prediction->reserved_5 = (void*) -1;
      return true;
    }


   //判断帧格式 如果不是 RGB 或者 BRG 表示无法处理
    cv::Mat image;
    if (input[0]->props.fmt == IVAS_VFMT_BGR8 || input[0]->props.fmt == IVAS_VFMT_RGB8)
      image = cv::Mat (input[0]->props.height, input[0]->props.width, CV_8UC3, indata, input[0]->props.stride);
    else {
      LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level, "Not supported format %d\n", input[0]->props.fmt);
      return -1;
    }

    //判断 输入图像尺寸 (非必须，vitis AI library 可以调用CPU来 resize)
    unsigned int width = kpriv->model->requiredwidth ();
    unsigned int height = kpriv->model->requiredheight ();
    if (width != inframe->props.width || height != inframe->props.height) {
      LOG_MESSAGE (LOG_LEVEL_WARNING, kpriv->log_level,"input image size is [%d,%d], model required size is [%d,%d]", 
      inframe->props.width,inframe->props.height, width, height);
      // return false; //TODO
    }

    //运行DPU
    ret = ivas_xrunmodel (kpriv, image, infer_meta, inframe);

    
    // 只会执行一次
    performanceTestStart(kpriv);
    // 每次都执行
    performanceTestRecord(kpriv);

    return ret;
  }

  int32_t xlnx_kernel_done (IVASKernel * handle)
  {

    ivas_xkpriv *kpriv = (ivas_xkpriv *) handle->kernel_priv;
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");
    return true;
  }

}
