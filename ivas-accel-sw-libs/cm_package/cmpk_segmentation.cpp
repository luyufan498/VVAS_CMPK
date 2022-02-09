#include "cmpk_segmentation.hpp"

namespace cmpk{


/*=========================================================*/
SegmentationData::SegmentationData(){};
SegmentationData::~SegmentationData(){};

std::shared_ptr<SegmentationData> SegmentationData::create(int input_width, 
                                                    int input_height,
                                                    int output_width,
                                                    int output_height, 
                                                    std::vector<SegmentationClass> seg_classlist) {
  return std::shared_ptr<SegmentationData>(new cmpk_SegmentationData( input_width, 
                                                                    input_height,
                                                                    output_width,
                                                                    output_height, 
                                                                    seg_classlist));
}


std::shared_ptr<SegmentationData> SegmentationData::create_debug() {
  return std::shared_ptr<SegmentationData>(new cmpk_SegmentationData());
}

/*=========================================================*/


cmpk_SegmentationData::cmpk_SegmentationData(int input_width, 
                                            int input_height,
                                            int output_width,
                                            int output_height, 
                                            std::vector<SegmentationClass> seg_classlist){

    this->input_width = input_width;
    this->input_height = input_height;
    this->output_width = output_width;
    this->output_height = output_height;
    this->seg_classlist.assign(seg_classlist.begin(), seg_classlist.end()); 
    
}


cmpk_SegmentationData::cmpk_SegmentationData(){

    // reserved_data_1 = -1;
}

cmpk_SegmentationData::~cmpk_SegmentationData(){
}

//需要保证线程安全
void cmpk_SegmentationData::mapDataSaveC1(cv::Mat map){
    std::lock_guard<std::mutex> lock(this->mtx_);
    map.copyTo(segMap_c1);
}

void cmpk_SegmentationData::mapDataSaveC3(cv::Mat map){
    std::lock_guard<std::mutex> lock(this->mtx_);
    map.copyTo(segMap_c3);
}


// //不允许外部读取
// cv::Mat cmpk_SegmentationData::readMapC1(){
//     if(segMap_c1.empty())
//         return NULL;
//     return segMap_c1;
// }

// //不允许外部读取
// cv::Mat cmpk_SegmentationData::readMapC3(bool needresize)
// {
//     if(segMap_c3.empty() || segMapResize_c3.empty())
//         return NULL;
//     if(needresize)
//         return segMapResize_c3;
//     else
//         return segMap_c3;
// }


//需要保证线程安全
cv::Mat cmpk_SegmentationData::mapC3Overly(cv::Mat frame){
    std::lock_guard<std::mutex> lock(this->mtx_);
    cv::Mat segMapResize_c3;
    cv::resize(segMap_c3, segMapResize_c3, cv::Size(frame.cols, frame.rows), 0, 0, cv::INTER_NEAREST);
    cv::addWeighted(segMapResize_c3,0.6,frame,0.4,0,segMapResize_c3);
    return segMapResize_c3;
};


SegmentationClass cmpk_SegmentationData::scenarioClassification(){
    std::lock_guard<std::mutex> lock(this->mtx_);
    
    SegmentationClass result;
    return result;
}


void cmpk_SegmentationData::writeCustomData(int data)
{
    // std::lock_guard<std::mutex> lock(this->mtx_);
    reserved_data_1 = data;
}


int cmpk_SegmentationData::readCustomData()
{
    // std::lock_guard<std::mutex> lock(this->mtx_);
    return reserved_data_1;
}



}