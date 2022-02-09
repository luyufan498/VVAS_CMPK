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

#define MAX_CLASS_LEN 1024
#define MAX_LABEL_LEN 1024
#define MAX_ALLOWED_CLASS 40
#define MAX_ALLOWED_LABELS 40
#define MULTABLELEN 32


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

  bool en_overlay; 
  
  int y_offset; // overlay area position
  int x_offset; // overlay area position

  int width;  // overlay area size
  int height; // overlay area size

  color back_color;
  bool en_back;
};

struct data_point{
  float data;
  int  y_converted;
};

struct sample_info
{
  int deviceid;
  int targetid;
  vector <data_point> points;
  int maxval; //w
  float y_ratio;
  int sample_interval_ms; // ms
  int refresh_interval_ms;
  int refresh_x_offset;

  int maxlen;
  int x_interval;
  float multable[MULTABLELEN];
  color line_color;
  yuv_color line_color_yuv;
};


struct overlaytext_info
{
  bool en_overlay;
  int y_offset;
  int x_offset;

  int line_height;
  int line_index;
  

};

struct performance_test
{
  int test_started = 0;
  unsigned long frames = 0;
  unsigned long last_displayed_frame = 0;
  long long timer_start;
  long long last_displayed_time;
  //记录时间长度
  double avgFPS = 0;
  long long excution_time_dpu = 0;

  vector<long long> displayed_time_list;
};

typedef struct performance_test ivas_perf;


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
  struct overlaytext_info textinfo;
  struct sample_info sampleinfo;
  
  bool performance_test;        /* enable/disable performance */
  struct performance_test pf;
  cmpk::fifocom ffc;                 /* FIFO communication*/

  int log_level;                /* LOG_LEVEL_ERROR=0, LOG_LEVEL_WARNING=1,
                                   LOG_LEVEL_INFO=2, LOG_LEVEL_DEBUG=3 */  
  mutable std::mutex mtx_;  // lock for multi threads
  
 
  int debug_param; // for debug
};






#endif /* __IVAS_XBOUNDINGBOX_H__  */