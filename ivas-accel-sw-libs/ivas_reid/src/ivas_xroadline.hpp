#pragma once
#include "ivas_xdpupriv.hpp"

#include <vitis/ai/lanedetect.hpp>
#include <vitis/ai/nnpp/lanedetect.hpp>

#include <iostream>


using namespace std;
using namespace cv;

class ivas_xroadline:public ivas_xdpumodel
{

    int log_level = 0;
    std::unique_ptr < vitis::ai::RoadLine > model;

public:

    // 这是初始化函数
    ivas_xroadline (ivas_xkpriv * kpriv, const std::string & model_name,
      bool need_preprocess);

    // 继承的函数
    virtual int run (ivas_xkpriv * kpriv, const cv::Mat & image,
      GstInferenceMeta * ivas_meta);

    // 都是继承的函数
    virtual int requiredwidth (void);
    virtual int requiredheight (void);
    virtual int close (void);
    virtual ~ ivas_xroadline ();
};
