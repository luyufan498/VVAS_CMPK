#ifndef __IVAS_CMPOSTSEG_H__
#define __IVAS_CMPOSTSEG_H__

#include <vector>
#include <stdio.h>
#include <string>
#include <ivas/ivaslogs.h>
#include <mutex>
#include "common.hpp"
#include "../../cm_package/cmpk_platformstats.hpp"
#include "../../cm_package/cmpk_ffc.hpp"
#include "../../cm_package/cmpk_chart.hpp"


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
};


struct power_info
{
  int hwmonid;
};




struct performance_info
{
  vector<long long> frames_displayed_time;
  bool enable = false;
  // 多久计算一次
  int  cal_time_ms = 1000;
  float avgFPS = 0;
  long long last_excution_time = 0;
};



 struct PowerChart:public cmpk::chart{
   public:
   virtual float sampleFuc(void *usrptr) override;
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
  struct performance_info perinfo;
  int log_level;                /* LOG_LEVEL_ERROR=0, LOG_LEVEL_WARNING=1,
                                   LOG_LEVEL_INFO=2, LOG_LEVEL_DEBUG=3 */  
  mutable std::mutex mtx_;  // lock for multi threads
  
  cmpk::fifocom ffc;   // send data out
  int debug_param; // for debug
  cmpk::chart *powerchart;
  int hwmonid;
};






#endif /* __IVAS_XBOUNDINGBOX_H__  */