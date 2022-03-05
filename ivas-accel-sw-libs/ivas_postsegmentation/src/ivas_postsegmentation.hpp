/*
 * @Author: CatMouse
 * @Date: 2022-01-08 14:48:01
 * @LastEditTime: 2022-01-28 15:19:53
 * @LastEditors: CatMouse
 * @Description: 
 * @FilePath: /VVAS/ivas-accel-sw-libs/ivas_postsegmentation/src/ivas_postsegmentation.hpp
 *  Copyright (c) 2021 CatMouse All rights reserved.
 */
#ifndef __IVAS_CMPOSTSEG_H__
#define __IVAS_CMPOSTSEG_H__

#include <vector>
#include <stdio.h>
#include <string>
#include <ivas/ivaslogs.h>
#include <mutex>
#include "common.hpp"
#include "../../cm_package/cmpk_ffc.hpp"

#define MAX_CLASS_LEN 1024
#define MAX_LABEL_LEN 1024
#define MAX_ALLOWED_CLASS 40
#define MAX_ALLOWED_LABELS 40

using namespace cv;
using namespace std;


struct color
{
  unsigned int blue;
  unsigned int green;
  unsigned int red;
};

struct  yuv_color
{
  unsigned char y;
  unsigned short uv;
  /* data */
};



struct ivass_xclassification
{
  color class_color;
  yuv_color converted_color;
  char class_name[MAX_CLASS_LEN];
  int class_id;
};


struct overlayframe_info
{
  IVASFrame *inframe;
  Mat image;
  Mat I420image;
  Mat NV12image;
  Mat lumaImg;
  Mat chromaImg;
  Mat lastSegImg;
  int y_offset;
  int x_offset;
  bool en_overlay;
  int width_overlay;
  int height_overlay;
};

struct scenario_info
{
  bool en_overlay;
  int y_offset;
  int x_offset;

  // bool en_ffc;
  // ivas_ffc ffc;
};

struct ivas_xoverlaypriv
{
  float font_size;
  unsigned int font;
  int line_thickness;
  int y_offset;
  color label_color;
  char label_filter[MAX_ALLOWED_LABELS][MAX_LABEL_LEN];
  unsigned char label_filter_cnt;
  unsigned short classes_count;
  ivass_xclassification class_list[MAX_ALLOWED_CLASS];
  struct overlayframe_info frameinfo;

  int log_level;                /* LOG_LEVEL_ERROR=0, LOG_LEVEL_WARNING=1,
                                   LOG_LEVEL_INFO=2, LOG_LEVEL_DEBUG=3 */  
  mutable std::mutex mtx_;
  scenario_info scenarioinfo;


  int debug_param;
  cmpk::fifocom ffc;

  std::string result_output_file;
  bool result2file;
};






#endif /* __IVAS_XBOUNDINGBOX_H__  */