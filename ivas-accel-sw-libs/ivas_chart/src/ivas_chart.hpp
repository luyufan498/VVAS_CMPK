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

enum
{
  
  CHART_MODE_LPD_TEMP = 0,
  CHART_MODE_FPD_TEMP= 1,
  CHART_MODE_PL_TEMP = 2,
  CHART_MODE_POWER = 3,
  CHART_MODE_FPS = 4,
  CHART_MODE_CUSTOM_LONG = 5,
  CHART_MODE_CUSTOM_FLOAT = 6
};

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


struct ivas_perf
{
  vector <long long> displayed_time_list;
  unsigned long frames = 0;
  int windowlen = 30; // 平滑帧数所用的窗口 （窗口大小会影响反应速度）
  //记录时间长度
  double avgFPS = 0;

  bool iswrite2file = false;
  string savefilepath;
  void writefps2file();
};



struct Sensor_info{
  string path;
  int hwmonid;
  float scale;
};


 struct ivas_chart:public cmpk::chart{
   public:
   virtual float sampleFuc(void *usrptr) override;
 };



struct ivas_xoverlaypriv
{
  mutable std::mutex mtx_;      // lock for multi threads
  int log_level;                /* LOG_LEVEL_ERROR=0, LOG_LEVEL_WARNING=1,
                                   LOG_LEVEL_INFO=2, LOG_LEVEL_DEBUG=3 */  
            // send data out

  int mode; // FPS=0, Sensor=1,
  
  bool fps_en;
  bool chart_anlysis_en;
  bool chart_title_en;
  bool chart_line_en;
  bool ffc_en;

  cmpk::fifocom ffc;
  cmpk::chart *cmpkchart;

  struct overlayframe_info frameinfo;
  struct ivas_perf perf;
  struct Sensor_info sensor;




  int debug_param; // for debug
};






#endif /* __IVAS_XBOUNDINGBOX_H__  */