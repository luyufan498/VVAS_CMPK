#pragma once
#include "ivas_xdpupriv.hpp"
#include <vitis/ai/segmentation.hpp>


using namespace std;
using namespace cv;

class ivas_xsegmentation:public ivas_xdpumodel
{

  int log_level = 0;
    std::unique_ptr <vitis::ai::Segmentation> model;

public:

    ivas_xsegmentation (ivas_xkpriv * kpriv, const std::string & model_name,
      bool need_preprocess);

  virtual int run (ivas_xkpriv * kpriv, const cv::Mat & image,
      GstInferenceMeta * ivas_meta);

  virtual int requiredwidth (void);
  virtual int requiredheight (void);
  virtual int close (void);

    virtual ~ ivas_xsegmentation ();
};
