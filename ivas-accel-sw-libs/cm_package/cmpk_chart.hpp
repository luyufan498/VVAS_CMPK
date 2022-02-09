/*
 * @Author: CatMouse
 * @Date: 2022-01-29 00:12:49
 * @LastEditTime: 2022-02-03 00:41:21
 * @LastEditors: CatMouse
 * @Description: 
 * @FilePath: /VVAS/ivas-accel-sw-libs/cm_package/cmpk_chart.hpp
 *  Copyright (c) 2021 CatMouse All rights reserved.
 */
#pragma once


#ifndef CMPK_CHART_H
#define CMPK_CHART_H

#include <vector>
#include <list> 
#include <stdio.h>
#include <string>
#include <mutex>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
namespace cmpk{


struct point{
  float data;
  /**
   * @brief 把data的值转换成y对应的位置, [0]
   * [0] 代表本身
   * [1] 代表偏移一个像素的值
   * [2] 代表偏移两个像素的值
   */
  vector <int>  y_converted;
};


struct rgb_color
{
  unsigned int blue=0;
  unsigned int green=0;
  unsigned int red=255;
};

struct  yuv_color
{
  unsigned char y;
  unsigned short uv;
  /* data */
};


struct chart
{

private:
    mutable std::mutex mtx_; /*for threads*/
    float x_interval = 1;

    long long timestamp_ms = 0;
    long timestamp_frame = 0,frame_cnt = 0;
    

    void updateDataAnalysis();
    void setXInterval();
    int fillYConvert(point *pre, point *nxt);
    int calFucX2Y(uint x);
    int dataScale2Y(float data);
    int drawStr(const char *str, int lnNum,cv::Mat lumaImg, cv::Mat chromaImg);
    int calFucIndex2XY(int index, int *x, int *y);

protected:
    float data_min = 0;
    float data_max = 0;
    float data_avg = 0;
    rgb_color line_rgb,label_rgb;
    yuv_color line_yuv,label_yuv;

    uint sample_interval_time = 30;
    uint sample_interval_frames = 10;
public:
    
    // data part
    vector <point> points;
    int maxlen = 0;
    
    // chart 
    uint chart_height = 0;
    uint chart_width = 0;

    // start pos of x and y
    uint chart_abs_x = 0;
    uint chart_abs_y = 0;


    float show_max_val  = 0;
    float show_min_val  = 0;


    uint line_thickness = 1;

    // amination
    uint refresh_offset = 0;

    /**
     * @brief 1 表示 时间触发 0表示 帧触发
     *  初始化的时间长时间触发使用 时间/短时间触发使用 帧
     */
    int sample_mode = 0;
    int chart_display_type = 0;

    int performance_optimize = 0;

    chart();
    chart(uint width, uint height,uint maxlen);
    virtual ~chart();
    
    
    int pushNewData (float data);
    virtual float sampleFuc(void *usrptr);
     
    /**
     * @brief Set the Line Color object
     * 
     * @param red 
     * @param green 
     * @param blue 
     */
    void setLineColor(uint red,uint green, uint blue);
    void setLableColor(uint red,uint green, uint blue);
    void setSampleIntervalTime(int interval_time_ms);


    int drawChartYUV(cv::Mat lumaImg, cv::Mat chromaImg);

    /**
     * @brief 使用填充的方式绘制波形图
     * 
     * @param lumaImg 
     * @param chromaImg 
     * @return int 
     */
    int drawChartYUVFill_unprotected(cv::Mat lumaImg, cv::Mat chromaImg,int mode);
    
    /**
     * @brief 使用画线的方式绘制波形图
     * 
     * @param lumaImg 
     * @param chromaImg 
     * @return int 
     */
    int drawChartYUVLine_unprotected(cv::Mat lumaImg, cv::Mat chromaImg, int mode);


    /**
     * @brief 更新数据，用户需要覆盖sampleFuc来实现更新数据点的功能
     * 
     * @param usrptr 用于传递用户数据
     * @return int 
     */
    int update(void *usrptr);
    
    std::string title_txt;
    int title_pos_x = 0;
    int title_pos_y = 0;
    float font_size;
    uint font;


    int drawTitle(cv::Mat lumaImg, cv::Mat chromaImg);
    int drawDataInfo(cv::Mat lumaImg, cv::Mat chromaImg);

    /* data */
};





}
#endif