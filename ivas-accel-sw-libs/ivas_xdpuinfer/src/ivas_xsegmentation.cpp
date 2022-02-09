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

extern "C"
{
#include <ivas/ivas_kernel.h>
}
#include "ivas_xsegmentation.hpp"



ivas_xsegmentation::ivas_xsegmentation (ivas_xkpriv * kpriv, const std::string & model_name,
    bool need_preprocess)
{
  // 查询log等级
  log_level = kpriv->log_level;
  // 进入函数
  LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");
  // 实例化model   
  model = vitis::ai::Segmentation::create (model_name, need_preprocess);
  // 退出函数
  LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "exit");
}

int
ivas_xsegmentation::run (ivas_xkpriv * kpriv, const cv::Mat & image,
    GstInferenceMeta * infer_meta)
{
    // LOG RUN IN
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");
    
    int cols = image.cols;
    int rows = image.rows;
    
    // report the size of the input image
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "Input size: %d %d" ,cols,rows);
    auto result = model->run_8UC1(image);


    IVASFrameProps out_props = {0, };
    out_props.width = result.segmentation.cols;
    out_props.height = result.segmentation.rows;
    out_props.fmt = IVAS_VFMT_BGR8;
    // if C3 : SIZE = W * H * 3
    // if C1 : SIZE = W * H
    uint32_t size = out_props.width*out_props.height;

    GstInferencePrediction *predict;

    IVASFrame *out_ivas_frame = ivas_alloc_buffer(kpriv->handle, size, IVAS_FRAME_MEMORY, &out_props);
    if (!out_ivas_frame)
    {
        LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level, "failed to alloc frame size: %d %d" ,out_props.width,out_props.height);
        return 0;
    }

    //无意义数据，仅仅是为了利用predict 函数
    BoundingBox bbox;
    bbox.x = 0;
    bbox.y = 0;
    bbox.width = 10;
    bbox.height = 10;
    predict = gst_inference_prediction_new_full (&bbox);
    
   

    GstInferenceClassification *c = 
      gst_inference_classification_new_full (10000, 0, "Segmentation", 0, NULL, NULL, NULL);

    gst_inference_prediction_append_classification (predict, c);

    int *classtag = (int *)malloc(sizeof(int));
     *classtag = 100;
    predict->reserved_1 = classtag;


    /*************************************************************/
    //把分配到的内存的指针指向当前的predict
    predict->sub_buffer =  (GstBuffer*)out_ivas_frame->app_priv;
    //map内存空间，准备开始写
    GstMapInfo info;
    gst_buffer_map((GstBuffer *)out_ivas_frame->app_priv, &info, GST_MAP_READWRITE);
    //拿出操作的内存指针
    char *indata = (char *)info.data;

    //用地址创建一个MAT 
    // CV_8UC1 OR CV_8UC3
    cv::Mat segMat(out_props.height, out_props.width, CV_8UC1, indata);
    // 拷贝数据到新开辟的内存空间中，用于传递
    // segMat = result.segmentation.clone();

    result.segmentation.copyTo(segMat);

    // 释放info
    gst_buffer_unmap((GstBuffer *)out_ivas_frame->app_priv, &info);
    /*************************************************************/


    gst_inference_prediction_append (infer_meta->prediction, predict);
    //对于segmentation,不考虑分类结果？
    
    out_ivas_frame->app_priv = NULL;
    ivas_free_buffer(kpriv->handle, out_ivas_frame);
    out_ivas_frame = NULL;
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "exit");
  return true;
}



int
ivas_xsegmentation::requiredwidth (void)
{
  LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "enter");
  return model->getInputWidth ();
}

int
ivas_xsegmentation::requiredheight (void)
{
  LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "enter");
  return model->getInputHeight ();
}

int
ivas_xsegmentation::close (void)
{
  LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "enter");
  return true;
}

ivas_xsegmentation::~ivas_xsegmentation ()
{
  LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "enter");
}
