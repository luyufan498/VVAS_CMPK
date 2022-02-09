#pragma once


#ifndef CMPK_STATIC_SGE_H
#define CMPK_STATIC_SGE_H


#include <fstream>
#include <iostream>
#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <tuple>
#include <mutex>
using namespace std;
using namespace cv;

namespace cmpk{


    typedef struct _SegmentationClass SegmentationClass;
    struct _SegmentationClass
    {
        std::string labelname;
        int classid;       
        int red;
        int green;
        int blue;
        int cnt;
    };



//用于存放多线程交换时候使用的数据
class SegmentationData{

public:


    SegmentationData();
    virtual ~SegmentationData();

    // 防止自动构造新的类
    SegmentationData(const SegmentationData &) = delete;
    SegmentationData &operator=(const SegmentationData &) = delete;
    
    // 构造新的指针
    // 类似于静态构造函数 只在初始化时候会创建一个新的类
    static std::shared_ptr<SegmentationData> create(int input_width, int input_height,int output_width,int output_height, std::vector<SegmentationClass> seg_classlist);
    static std::shared_ptr<SegmentationData> create_debug();

    // 保存seg结果(有锁)
    // Map 保存在私有成员中，不公开访问
    virtual void mapDataSaveC1(cv::Mat map) = 0;
    virtual void mapDataSaveC3(cv::Mat map) = 0;


    
    // 绘图 （有锁）
    // 内部的原始数据作为私有数据，不能被直接访问
    virtual cv::Mat mapC3Overly(cv::Mat frame) = 0;

    virtual SegmentationClass scenarioClassification() = 0;
    // virtual cv::Mat readMapC1();
    // virtual cv::Mat readMapC3(bool needresize);

    virtual void writeCustomData(int data) = 0;
    virtual int readCustomData() = 0;
};

class cmpk_SegmentationData: public SegmentationData {

public:

    // 防止自动构造新的类
    cmpk_SegmentationData(const cmpk_SegmentationData &) = delete;
    cmpk_SegmentationData &operator=(const cmpk_SegmentationData &) = delete;

    cmpk_SegmentationData(int input_width, int input_height,int output_width,int output_height, std::vector<SegmentationClass> seg_classlist);
    cmpk_SegmentationData();
    virtual ~cmpk_SegmentationData();

    virtual void mapDataSaveC1(cv::Mat map) override;
    virtual void mapDataSaveC3(cv::Mat map) override;
    virtual SegmentationClass scenarioClassification() override;
    // virtual cv::Mat readMapC1() ;
    // virtual cv::Mat readMapC3(bool needresize) ;

    virtual cv::Mat mapC3Overly(cv::Mat frame) override;
    virtual void writeCustomData(int data) override;
    virtual int readCustomData() override;


private:
    bool saved_c1 = 0, saved_c3 = 0;
    //keep input and output size
    int input_width = 0,input_height = 0,output_width = 0,output_height = 0;
    //keep input d
    std::vector<SegmentationClass> seg_classlist;
    //keep resized map
    cv::Mat segMap_c1,segMap_c3; 

    //lock for multi thread
    mutable std::mutex mtx_;

    //for debug and custom
    int reserved_data_1;
};





}


#endif