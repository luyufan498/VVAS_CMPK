/*
 * @Author: CatMouse
 * @Date: 2022-01-07 14:23:06
 * @LastEditTime: 2022-01-28 15:25:26
 * @LastEditors: CatMouse
 * @Description: 
 * @FilePath: /VVAS/ivas-accel-sw-libs/cm_package/cmpk_ffc.hpp
 * 可以输入预定的版权声明、个性签名、空行等
 */
#ifndef CMPK_FFC_H
#define CMPK_FFC_H

#include <stdio.h>
#include <string>
#include <vector>
#define FIFO_WRITE "/tmp/ivasfifo_tomain"
#define FIFO_READ "/tmp/ivasfifo_frommain"
// #define MAX_LEN 128
#define FIFO_MAX_LEN 128

namespace cmpk{

struct model_info
{   
    std::string model_name;
    std::string model_class;
    std::string model_path;
};


struct fifocommuncation_control{
  std::string txpath;
  std::string rxpath;
  int tx_frame_interval = 10; // unit:frames
  int rx_frame_interval;
  bool report_fps;

  // 新版本
  std::vector<std::string> lines_buffer; //读取的行会被临时存放的到这个里面  
  struct model_info modelinfo;
};
typedef struct fifocommuncation_control fifocom;


typedef enum
{
  IVAS_FFC_REPORT_FPS,
  IVAS_FFC_REPORT_PREDICTION,
  IVAS_FFC_REPORT_CUSTOM, 
}IvasFFCReportType;


typedef enum
{
  IVAS_FFC_CONTROL_HIGHER_PRF,
  IVAS_FFC_CONTROL_LOWER_PRE,
  IVAS_FFC_CONTROL_REPORT_MODEL,

  IVAS_FFC_CONTROL_UNKNOWN
}IvasFFCControlType;

//not use
int ivas_fifocommuncation_send(char *data,int len,IvasFFCReportType sendtype);
int ivas_fifocommuncation_send_raw(fifocom ffc,const char *data,int len);
int ivas_fifocommuncation_read_raw(fifocom ffc,char *data,int len);
IvasFFCControlType ivas_fifocommuncation_read_ctr(fifocom ffc);


/**
 * @description: 新版本 清空上次缓存的内容后从FIFO中读取数据
 * @param {fifocom} *ffc 存放控制信息的结构体
 * @return {int} 返回读取到分割字符串的数量 如果为0说明没有读到
 */
int fifoComRead(fifocom *ffc);

// /**
//  * @description: 新版本 根据BUFF来更新模型数据
//  * 需要先调用fifoComRead来更新缓冲区 
//  * 这个函数不会检查任何的数值正确性
//  * @param {fifocom} *ffc 存放控制信息的结构体
//  * @return {*} 如果返回 1 表示更新成功 -1 表示失败
//  */
// int fifoComCtr_DynamicModel(fifocom *ffc);


int fifoComReadNB_splitbuffers(fifocom *ffc);

int fifoComWriteNB_string(fifocom *ffc, std::string str);
int fifoComWriteNB_string(fifocom *ffc, const char* str, int len);
int fifoComWriteNB_string(fifocom *ffc, const char* str);

/**
 * @description: 用于汇报FPS，FIFO中检查 reportFPS 这个头即可 
 * @param {fifocom} *ffc
 * @param {float} fps 帧数
 * @return {*}
 */
int fifoComReportNB_fps(fifocom *ffc,float fps);


/**
 * @description: 用于汇报FPS，host检查 传递的字符串 "reportFPS" 这个头即可 
 * @param {fifocom} *ffc
 * @param {float} fps
 * @param {char} *info
 * @return {*}
 */
int fifoComReportNB_fps(fifocom *ffc,float fps,const char *info);


}

#endif