/*
 * @Author: CatMouse
 * @Date: 2022-01-29 00:48:38
 * @LastEditTime: 2022-02-03 02:03:42
 * @LastEditors: CatMouse
 * @Description: 
 * @FilePath: /VVAS/ivas-accel-sw-libs/cm_package/cmpk_chart.cpp
 *  Copyright (c) 2021 CatMouse All rights reserved.
 */
#include "cmpk_chart.hpp"
#include <sys/time.h>
namespace cmpk{

/**
 * @brief Get the time object 转换成了 ms 了已经
 * 
 * @return long long 返回时间 ms
 */
long long
get_time ()
{
  struct timeval tv;
  gettimeofday (&tv, NULL);
  return ((long long) tv.tv_sec * 1000 + tv.tv_usec/1000) +
      42 * 60 * 60 * INT64_C (1000);
}



//色彩转换
yuv_color
convert_rgb_to_yuv(rgb_color clr)
{
  unsigned char y;
  unsigned short uv;
  Mat YUVmat;
  Mat BGRmat (2, 2, CV_8UC3, Scalar (clr.red, clr.green, clr.blue));
  cvtColor (BGRmat, YUVmat, cv::COLOR_BGR2YUV_I420);
  y = YUVmat.at < uchar > (0, 0);
  uv = YUVmat.at < uchar > (2, 0) << 8 | YUVmat.at < uchar > (2, 1);
  yuv_color newyuv;
  newyuv.y = y;
  newyuv.uv = uv;
  return newyuv;
}


chart::chart(uint width, uint height ,uint maxlen)
{
    this->chart_width = width;
    this->chart_height = height;
    this->chart_abs_x = 0;
    this->chart_abs_y = 0;
    this->maxlen = maxlen;
    this->setXInterval();

}


chart::chart(){
    this->chart_width = 512;
    this->chart_height = 256;
    this->chart_abs_x = 0;
    this->chart_abs_y = 0;   
    this->maxlen = 32;

    this->show_max_val = 100;
    this->show_min_val = 0;

    this->setSampleIntervalTime(500);
    this->setLableColor(0,0,0);
    this->setLineColor(255,0,0);
    this->setXInterval();
}

void chart::setXInterval(){    
    if(this->chart_width>0 && this->maxlen>0)
    {
        this->x_interval = this->chart_width/this->maxlen;
    }
    
}


void chart::setLineColor(uint red,uint green, uint blue)
{
    this->line_rgb.red =red;
    this->line_rgb.green = green;
    this->line_rgb.blue = blue;

    cv::Mat YUVmat;
    cv::Mat BGRmat (2, 2, CV_8UC3, Scalar (red, green, blue));
    cv::cvtColor (BGRmat, YUVmat, cv::COLOR_BGR2YUV_I420);

    this->line_yuv.y = YUVmat.at < uchar > (0, 0);
    this->line_yuv.uv = YUVmat.at < uchar > (2, 0) << 8 | YUVmat.at < uchar > (2, 1);
}


void chart::setLableColor(uint red,uint green, uint blue)
{
    this->label_rgb.red =red;
    this->label_rgb.green = green;
    this->label_rgb.blue = blue;

    cv::Mat YUVmat;
    cv::Mat BGRmat (2, 2, CV_8UC3, Scalar (red, green, blue));
    cv::cvtColor (BGRmat, YUVmat, cv::COLOR_BGR2YUV_I420);

    this->label_yuv.y = YUVmat.at < uchar > (0, 0);
    this->label_yuv.uv = YUVmat.at < uchar > (2, 0) << 8 | YUVmat.at < uchar > (2, 1);
}




/**
 * @brief  主要的作用是自动设置类型 0 表示使用 帧来计算间隔，1表示用时间来计算间隔
 * 
 * @param interval_time_ms 
 */
void chart::setSampleIntervalTime(int interval_time_ms){
    
    if(interval_time_ms < 1000){

        this->sample_mode  = 0; // 帧来计算 间隔
    }
    else
    {
        this->sample_mode  = 1; // 时间来计算间隔
    }

    //默认设置为30帧
    this->sample_interval_frames = (int)(interval_time_ms/33.3);
    this->sample_interval_time = interval_time_ms;
}






int chart::dataScale2Y(float data){
    if(data < this->show_min_val)
        return 0;
    if(data > this->show_max_val)
        return this->chart_height;

    float newdata = data - this->show_min_val;
    int y_converted = this->chart_height  -  newdata  * this->chart_height / (show_max_val - show_min_val);
    return y_converted;
}


int chart::fillYConvert(point *pre, point *nxt){

    int len = (int) (this->x_interval + 1);
    if(len < 2)
        return 0;
    
    int y_pre = pre->y_converted[0];
    int y_nxt = nxt->y_converted[0];

    for(auto i = 1; i< len ; i++){
        int newy = i/this->x_interval * (y_nxt - y_pre) + y_pre;
        pre->y_converted.push_back(newy);
    }

    return 0;
    
}


void chart::updateDataAnalysis(){
    if(this->points.size()<1){
        return ;
    }
    
    float sum = this->points[0].data;
    float min = this->points[0].data;
    float max = this->points[0].data;

    for(int i=1;(long unsigned int)i< this->points.size();i++){
        float data = this->points[i].data;
        sum += data;
        if(data >  max)
            max = data;
        
        if(data < min)
            min = data;
    }

    this->data_min = min;
    this->data_max = max;
    this->data_avg = sum / (int)this->points.size();
}



int chart::pushNewData(float data){


    this->setXInterval();
    point newpoint;
    newpoint.data = data;
    newpoint.y_converted.push_back(this->dataScale2Y(data));

    std::lock_guard<std::mutex> lock(this->mtx_);
    
    if(this->points.size()>0)
        this->fillYConvert( &(this->points.back()), &newpoint);
    
    // 存放到数据表中
    this->points.push_back(newpoint);
    // 保证画的时候头尾有存放正确的点
    if(this->points.size() > (long unsigned int)this->maxlen + 2){
        this->points.erase(this->points.begin());
    }

    this->updateDataAnalysis();

    return 0;
}

inline
int chart::calFucX2Y(uint x){
    
   
    int index,offset;

    if(this->points.size() < (long unsigned int)this->maxlen + 2)
    {
        index =  (int) ((x)  /  this->x_interval);
        offset = (int) ((x)  - index * this->x_interval);   

        if((long unsigned int)index + 2 >  this->points.size())
            return -1;
        else
        if((long unsigned int)index + 2 ==  this->points.size() && offset > (int)this->refresh_offset)
            return -1;
    }
    else
    {
        index =  (int) ((x + this->refresh_offset)  /  this->x_interval);
        offset = (int) ((x + this->refresh_offset)  - index * this->x_interval);   
    
    }

    if(this->points[index].y_converted.size() > (long unsigned int)offset + 1)
    {
        return this->points[index].y_converted[offset];
    }
    else
    {
        return this->points[index].y_converted.back();
    }
}


int chart::calFucIndex2XY(int index, int *x, int *y){

    if((long unsigned int)index >= this->points.size() || index >= this->maxlen)
        return -1;
    
    int left_offset = this->refresh_offset;

    if((long unsigned int)this->maxlen + 2 > this->points.size()){
        left_offset = 0;
    }

    if(index <= 0){
        *x = 0;
        *y = this->points[0].y_converted[left_offset];
    }
    else
    if((long unsigned int)index + 1 == this->points.size() )
    {
       *x =(int) index * this->x_interval;
       *y = this->points[index-1].y_converted[this->refresh_offset];
    }
    else if ((long unsigned int)index + 1 < this->points.size()){
        *x = (int) index * this->x_interval - left_offset;     
        *y = this->points[index].y_converted[0];
    } 

    return 0;
}



int chart::update(void *usrptr){
    this->setXInterval();
    
    long long timestamp_ms = this->timestamp_ms;
   
  

    long long now_ms = get_time();
    
    if(this->sample_mode == 0)
    {       
      

        if(this->frame_cnt - this->timestamp_frame >= this->sample_interval_frames){
            auto sampleval = this->sampleFuc(usrptr);
            this->pushNewData(sampleval);
            this->timestamp_frame = this->frame_cnt;
        }
        
        this->refresh_offset = (uint)(this->x_interval * (this->frame_cnt - this->timestamp_frame) /this->sample_interval_frames);
        
        // cout<<this->refresh_offset<<"-"<<this->x_interval<<"  f_cnt:"<<frame_cnt<<","<<now_frame-timestamp_frame<<" Max Interval:"<<this->sample_interval_frames<<endl;
    }
    else
    {
        
        if(now_ms - timestamp_ms  >=  this->sample_interval_time)
        {
            this->pushNewData(this->sampleFuc(usrptr));
            this->timestamp_ms = now_ms;
        }
       
        
        this->refresh_offset = (uint)(this->x_interval * (now_ms-timestamp_ms) /this->sample_interval_time);
        
    }
    
    this->frame_cnt ++;
    return 0;

}


float chart::sampleFuc(void *usrptr){
    std::cout<<"Error: this func should never be called"<<endl;
    return 0;
}

/**
 * @brief 不同方式填充原有像素 不同模式会显著影响性能
 * 
 * @param chromax 色度图的x  NV12 下 Y:UV= 4：1
 * @param chromay 色图图的y
 * @param lumaImg 明度图
 * @param chromaImg 色图图
 * @param yuv_clr 颜色
 * @param mode 0:4 模式决定填充方式
 *  0 不填充明度图
 *  1 填充1/4
 *  2 填充2/4
 *  3 填充3/4
 *  4 完全覆盖
 */
inline
void drawPointNV12ChromaxBase(int chromax, int chromay,cv::Mat lumaImg, cv::Mat chromaImg,yuv_color  yuv_clr, int mode){
    static int x_offset[4] = {0,1,0,1};
    static int y_offset[4] = {0,0,1,1};
    int lumaImgx = chromax*2;
    int lumaImgy = chromay*2;
    for(auto i=0;i< mode ;i++)
    {
        lumaImg.at<unsigned char>(lumaImgy+y_offset[i], lumaImgx+x_offset[i]) = yuv_clr.y;
    }
     
    chromaImg.at<unsigned short>(chromay, chromax) = yuv_clr.uv;
}



int chart::drawChartYUVFill_unprotected(cv::Mat lumaImg, cv::Mat chromaImg, int mode){
 
    this->setXInterval();
    int width = this->chart_width;
    int heigh = this->chart_height;
    // 只画1/4的像素 更快
    for(auto x=0;x<width;x+=2){
        int y_converted = this->calFucX2Y(x);
        if(y_converted<0) return 0;
        //-----------------------------//
        int lumaImgx = x + this->chart_abs_x;
        int lumaImgy = y_converted + this->chart_abs_y;
        int chromax = lumaImgx/2;
        int y_max =  (int)this->chart_abs_y + heigh;

        switch (mode)
        {
        case 3/* constant-expression */:
            {
               // 同样只画1/4的像素
                for(auto y = lumaImgy; y< y_max;y+=2)
                {
                    chromaImg.at<unsigned short>(y/2, chromax) = this->line_yuv.uv;
                }
            }
            break;

        case 2:
            {
                for(auto y = lumaImgy; y< y_max;y+=2)
                {
                    
                    lumaImg.at<unsigned char>(lumaImgy, lumaImgx) = this->line_yuv.y;
                    chromaImg.at<unsigned short>(y/2, chromax) = this->line_yuv.uv;
                }
            }
            break;
        case 1:
            {
                for(auto y = lumaImgy; y< y_max;y+=2)
                {
                    lumaImg.at<unsigned char>(lumaImgy, lumaImgx) = this->line_yuv.y;
                    lumaImg.at<unsigned char>(lumaImgy + 1, lumaImgx + 1) = this->line_yuv.y;
                    chromaImg.at<unsigned short>(y/2, chromax) = this->line_yuv.uv;
                }
            }
            break;
        
        case 0:
            {
                for(auto y = lumaImgy; y< y_max;y+=2)
                {
                    lumaImg.at<unsigned char>(lumaImgy, lumaImgx) = this->line_yuv.y;
                    lumaImg.at<unsigned char>(lumaImgy + 1, lumaImgx + 1) = this->line_yuv.y;
                    lumaImg.at<unsigned char>(lumaImgy + 1, lumaImgx) = this->line_yuv.y;
                    lumaImg.at<unsigned char>(lumaImgy + 1, lumaImgx + 1) = this->line_yuv.y;
                    chromaImg.at<unsigned short>(y/2, chromax) = this->line_yuv.uv;
                }
            }
            break;


        default:
            {
                // 同样只画1/4的像素
                for(auto y = lumaImgy; y< y_max;y+=2)
                {
                    lumaImg.at<unsigned char>(lumaImgy, lumaImgx) = this->line_yuv.y;
                    chromaImg.at<unsigned short>(y/2, chromax) = this->line_yuv.uv;
                }
            }
            break;
        }
    }

    return 0;
}

int chart::drawChartYUVLine_unprotected(cv::Mat lumaImg, cv::Mat chromaImg, int mode){
    
    this->setXInterval();
    //多线程在外面保护
    int width = (int)this->chart_width;
    int y_last = calFucX2Y(0) + this->chart_abs_y;
    int x_last = 0 + this->chart_abs_x;
    switch (mode)
    {
    // 57 - 80
    case 2:
        {
            for(auto x=0;x<width;x+=2){
                int y_converted = this->calFucX2Y(x);
                if(y_converted<0) return 0;
                //-----------------------------//
                  
                int chromaImgx = (x + this->chart_abs_x)/2;
                int chromaImgy = (y_converted + this->chart_abs_y)/2;

                int lumaImgx = chromaImgx * 2;
                int lumaImgy = chromaImgy * 2;     
                
                lumaImg.at<unsigned char>(lumaImgy, lumaImgx) = this->line_yuv.y; 
                lumaImg.at<unsigned char>(lumaImgy, lumaImgx + 1) = this->line_yuv.y; 
                lumaImg.at<unsigned char>(lumaImgy + 1, lumaImgx) = this->line_yuv.y; 
                lumaImg.at<unsigned char>(lumaImgy + 1, lumaImgx + 1) = this->line_yuv.y; 
                chromaImg.at<unsigned short>(chromaImgy, chromaImgx) = this->line_yuv.uv;            
            }
        }
        break; 

    case 1:
        {
            //  描第一个点
            // 65 - 70
            int left_offset = this->refresh_offset;
            

            if(this->points.size() < (long unsigned int)this->maxlen + 2){
                left_offset = 0;
            }

            // 除了头尾 只描固定采样点
            for(auto index = 1; (long unsigned int)index < this->points.size(); index++){
                int x,y;
                if((long unsigned int)index + 1 == this->points.size()){
                    x = this->refresh_offset + (int) this->x_interval * (index - 1);
                    y = this->points[index - 1].y_converted[this->refresh_offset];
                    
                    if(x > width - 1){
                        x = width -1;
                        y = this->calFucX2Y(x);
                    }
                }
                else
                {
                    x = (int) (this->x_interval * index - left_offset); 
                    y = this->points[index].y_converted[0];
                     
                }
                
                int lumaImgx = x + this->chart_abs_x;
                int lumaImgy = y + this->chart_abs_y;        

                // cv::line(lumaImg,Point(x_last,y_last), Point(lumaImgx,lumaImgy),Scalar (this->line_yuv.y),this->line_thickness*2);
                cv::line(chromaImg,Point(x_last/2,y_last/2), Point(lumaImgx/2,lumaImgy/2),Scalar (this->line_yuv.uv),this->line_thickness);
               
                y_last = lumaImgy;
                x_last = lumaImgx;
            }
        }
        break;
    
    case 0:
        {  
            //  描第一个点 
            // 200 us
            
            int left_offset = this->refresh_offset;
           

            if(this->points.size() < (long unsigned int)this->maxlen + 2){
                left_offset = 0;
                
            }

            // 除了头尾 只描固定采样点
            for(auto index = 1; (long unsigned int)index < this->points.size(); index++){
                int x,y;
                if((long unsigned int)index + 1 == this->points.size()){
                    x = this->refresh_offset + (int) this->x_interval * (index - 1);
                    y = this->points[index - 1].y_converted[this->refresh_offset];
                    
                    if(x > width - 1){
                        x = width -1;
                        y = this->calFucX2Y(x);
                    }
                }
                else
                {
                    x = (int) (this->x_interval * index - left_offset); 
                    y = this->points[index].y_converted[0];
                     
                }
                
                int lumaImgx = x + this->chart_abs_x;
                int lumaImgy = y + this->chart_abs_y;        

                cv::line(lumaImg,Point(x_last,y_last), Point(lumaImgx,lumaImgy),Scalar (this->line_yuv.y),this->line_thickness*2);
                cv::line(chromaImg,Point(x_last/2,y_last/2), Point(lumaImgx/2,lumaImgy/2),Scalar (this->line_yuv.uv),this->line_thickness);
               
                y_last = lumaImgy;
                x_last = lumaImgx;
            }

        }
        break;

    case 3:
        // 40 us
        {
            for(auto x=0;x<width;x+=2){
                int y_converted = this->calFucX2Y(x);
                if(y_converted<0) return 0;
                //-----------------------------//
                int lumaImgx = x + this->chart_abs_x;
                int lumaImgy = y_converted + this->chart_abs_y;     
                lumaImg.at<unsigned char>(lumaImgy, lumaImgx) = this->line_yuv.y;    
                chromaImg.at<unsigned short>(lumaImgy/2, lumaImgx/2) = this->line_yuv.uv;            
            }
        }
        break; 

    
    default:
        {
            for(auto x=2;x<width;x+=2){
                int y_converted = this->calFucX2Y(x);
                if(y_converted<0) return 0;
                //-----------------------------//
                int lumaImgx = x + this->chart_abs_x;
                int lumaImgy = y_converted + this->chart_abs_y;        

                cv::line(chromaImg,Point(lumaImgx/2 -1,y_last/2), Point(lumaImgx/2,lumaImgy/2),Scalar (this->line_yuv.uv),1);
                y_last = y_converted;
            }
        }
        break;
    }



    return 0;

}



int chart::drawStr(const char *str, int lnNum,cv::Mat lumaImg, cv::Mat chromaImg){
    int baseline;
    Size textsize;
    textsize = getTextSize ("A", this->font, this->font_size, 1, &baseline);

    int x = this->title_pos_x + this->chart_abs_x;
    int y = 1.5 * (lnNum + 1) * textsize.height +  this->title_pos_y + this->chart_abs_y;
    putText (lumaImg,   str, cv::Point (x,y), this->font, this->font_size,Scalar (this->label_yuv.y), 1, 1);
    putText (chromaImg, str, cv::Point ((x)/ 2,y/2), this->font,this->font_size / 2, Scalar (this->label_yuv.uv), 1, 1);
    return 0;
}



int chart::drawTitle(cv::Mat lumaImg, cv::Mat chromaImg){
 
    return  this->drawStr(this->title_txt.c_str(),0,lumaImg,chromaImg);
}

int chart::drawDataInfo(cv::Mat lumaImg, cv::Mat chromaImg){    
    char buff[60]={0};
    sprintf(buff,"Max:%3.2f Min:%3.2f Avg:%3.2f",this->data_max,this->data_min,this->data_avg);
    return  this->drawStr(buff,1,lumaImg,chromaImg);
}


int chart::drawChartYUV(cv::Mat lumaImg, cv::Mat chromaImg){

    switch (this->chart_display_type)
    {
    case 0:
        return this->drawChartYUVLine_unprotected(lumaImg,chromaImg,this->performance_optimize);
        break;
    case 1:
        return this->drawChartYUVFill_unprotected(lumaImg,chromaImg,this->performance_optimize);
        break;
    default:
        return this->drawChartYUVLine_unprotected(lumaImg,chromaImg,this->performance_optimize);
        break;
    }
}



chart::~chart(){};


}