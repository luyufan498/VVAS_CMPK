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

#include "ivas_xroadline.hpp"

//初始化类
ivas_xroadline::ivas_xroadline (ivas_xkpriv * kpriv, const std::string & model_name,
    bool need_preprocess)
{
  // 查询log等级
  log_level = kpriv->log_level;
  // 进入函数
  LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");
  // 实例化model   
  model = vitis::ai::RoadLine::create (model_name, need_preprocess);
  // 退出函数
  LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "exit");
}


// 运行model
int
ivas_xroadline::run (ivas_xkpriv * kpriv, const cv::Mat & image,
    GstInferenceMeta * infer_meta)
{

  LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");



  int cols = image.cols;
  int rows = image.rows;

  LOG_MESSAGE (LOG_LEVEL_INFO, kpriv->log_level, "Input size: %d %d"
  ,cols,rows);

  if (NULL == infer_meta->prediction) {
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");
    infer_meta->prediction = gst_inference_prediction_new ();
    infer_meta->prediction->bbox.width = cols;
    infer_meta->prediction->bbox.height = rows;
  } else {
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "enter");
    infer_meta->prediction->bbox.width = cols;
    infer_meta->prediction->bbox.height = rows;
  }

  auto results = model->run (image);
     

    auto index = 0;
    for (auto &line : results.lines){
        vector<Point> points_poly = line.points_cluster;
        int type = line.type < 5 ? line.type : 5;
        
        // 类型2 
        if (type == 2 && points_poly[0].x < image.rows * 0.5)
        continue;

        for(auto &point:points_poly){
            LOG_MESSAGE (LOG_LEVEL_INFO, kpriv->log_level, "Line:%d (%d,%d)",index,point.x,point.y);
        
                int label = type;
                float xmin = point.x ;
                float ymin = point.y ;
                float xmax = point.x + 3;
                float ymax = point.y + 3;
                if (xmax > cols)
                    xmax = cols;
                if (ymax > rows)
                    ymax = rows;
        
        
                BoundingBox bbox;
                GstInferencePrediction *predict;
                GstInferenceClassification *c = NULL;
                bbox.x = xmin;
                bbox.y = ymin;
                bbox.width = xmax - xmin;
                bbox.height = ymax - ymin;
                predict = gst_inference_prediction_new_full (&bbox);
                c = gst_inference_classification_new_full (label, 1,
        NULL, 0, NULL, NULL, NULL);
                gst_inference_prediction_append_classification (predict, c);
                gst_inference_prediction_append (infer_meta->prediction, predict);
        }
        index ++;

    }


    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "exit");
    return true;
}


int
ivas_xroadline::requiredwidth (void)
{
  LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "enter");
  return model->getInputWidth ();
}

int
ivas_xroadline::requiredheight (void)
{
  LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "enter");
  return model->getInputHeight ();
}

int
ivas_xroadline::close (void)
{
  LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "enter");
  return true;
}

ivas_xroadline::~ivas_xroadline ()
{
  LOG_MESSAGE (LOG_LEVEL_DEBUG, log_level, "enter");
}
