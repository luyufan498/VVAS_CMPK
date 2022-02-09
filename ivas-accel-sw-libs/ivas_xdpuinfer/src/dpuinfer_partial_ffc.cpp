/*
 * @Author: CatMouse
 * @Date: 2022-01-17 21:49:56
 * @LastEditTime: 2022-01-18 13:09:48
 * @LastEditors: CatMouse
 * @Description: 
 * @FilePath: /VVAS/ivas-accel-sw-libs/ivas_xdpuinfer/src/dpuinfer_partial_ffc.cpp
 *  Copyright (c) 2021 CatMouse All rights reserved.
 */

#include "dpuinfer_partial_ffc.hpp"
#include "dpuinfer_partial_model.hpp"
#include "../../cm_package/cmpk_ffc.hpp"
#include <string>
#include <sys/stat.h>

using namespace cv;
using namespace std;
using namespace cmpk;

inline bool
fileexists (const string & name)
{
  struct stat buffer;
  return (stat (name.c_str (), &buffer) == 0);
}


/**
 * @description: 读取AI处理间隔的帧数
 * @param {ivas_xkpriv *} kpriv
 * @param {fifocom} *ffc
 * @return {*}
 */
int fifoComCtr_DPUInvteral(ivas_xkpriv * kpriv)
{
    std::string header = "pluginCtr_invteral";

    cmpk::fifocom *ffc = &kpriv->ffc;
    if(ffc->lines_buffer.size()<(1+1))
    {
        return -1;
    } 

    if(ffc->lines_buffer[0].compare(header))
    {
        return -1;
    }

    string num_string = ffc->lines_buffer[1];
    int value = atoi(num_string.c_str());

    //非法值
    if(value <= 0)
    {
        
        LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level, "ffc:invail interval value %s --> %d",ffc->lines_buffer[1].c_str(),value);
        return -1;
    }

    kpriv->interval_frames = value;

    // cout<< "reset interval value "<<ffc->lines_buffer[1]<<" "<<value<<endl;
    LOG_MESSAGE (LOG_LEVEL_DEBUG, kpriv->log_level, "reset interval value %d",value);

    return 1;
}


/**
 * @description: 新版本 根据BUFF来更新模型数据
 * 需要先调用fifoComRead来更新缓冲区 
 * 这个函数不会检查任何的数值正确性
 * @param {fifocom} *ffc 存放控制信息的结构体
 * @return {*} 如果返回 1 表示更新成功 -1 表示失败
 */
int fifoComCtr_DynamicModel(fifocom *ffc){
    std::string header = "switch2model";
   
    if(ffc->lines_buffer.size()<(3+1))
    {
        return false;
    } 

    if(ffc->lines_buffer[0].compare(header))
    {
        return false;
    }


    ffc->modelinfo.model_name = ffc->lines_buffer[1];
    ffc->modelinfo.model_class = ffc->lines_buffer[2];
    ffc->modelinfo.model_path = ffc->lines_buffer[3];

    

    if (!fileexists (ffc->modelinfo.model_path))
    {
        //检查一下第三个参数是不是路径
        return false;
    }

    cout<< "FIFO Model Read"<<ffc->modelinfo.model_name<<" "<<ffc->modelinfo.model_class
    <<" "<<ffc->modelinfo.model_path<<endl;

    return true;
}


/**
 * @description: 载入模型，需要使用 fifoComRead 先读取 刷新fifo的内容，每帧只刷新一次
 * @param {ivas_xkpriv *} kpriv
 * @return {*} 返回 0 表示没有执行，-1表示失败，1表示成功
 */
int loadDynamicModelfromFFC(ivas_xkpriv * kpriv)
{ 
  
    if(!kpriv->run_time_model)
    {
        return 0;
    }

    cmpk::fifocom *ffc = &kpriv->ffc;
    
    if(fifoComCtr_DynamicModel(ffc))
    {

      ivas_xkpriv *tmpxkpriv = (ivas_xkpriv *) calloc (1, sizeof (ivas_xkpriv));
    //   ivas_xkpriv tmpxkpriv;
      // 修改FFC函数可以无限添加参数
      tmpxkpriv->modelname = ffc->modelinfo.model_name;
      tmpxkpriv->modelpath = ffc->modelinfo.model_path;
      tmpxkpriv->modelclass = ivas_xclass_to_num((char*)ffc->modelinfo.model_class.c_str());

   
      tmpxkpriv->elfname =  modelexits (tmpxkpriv);
      if (tmpxkpriv->elfname.empty ()) {
        LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level,"dynamic model:%s check failed\n",tmpxkpriv->modelname.c_str());
        return -1;
      }

      tmpxkpriv->need_preprocess = kpriv->need_preprocess;

      // 创建一个，确保没问题再移植
      tmpxkpriv->model = ivas_xinitmodel (tmpxkpriv, tmpxkpriv->modelclass);

      LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level,"enter %p",tmpxkpriv->model);


      if(tmpxkpriv->model == NULL){
        LOG_MESSAGE (LOG_LEVEL_ERROR, kpriv->log_level,"dynamic model:%s intt failed\n",tmpxkpriv->modelname.c_str());
        return -1;
      }

      cout<< tmpxkpriv->modelpath <<endl;
      cout<< tmpxkpriv->modelname <<endl;
      cout<< tmpxkpriv->modelclass <<endl;
      cout<< tmpxkpriv->elfname <<endl;

      //清空model 和 label
      ivas_clean_currentmodel(kpriv);

      //全部移植过去

      // 移植model本身
      kpriv->modelname = tmpxkpriv->modelname;
      kpriv->modelpath = tmpxkpriv->modelpath;
      kpriv->modelclass = tmpxkpriv->modelclass;
      kpriv->elfname = tmpxkpriv->elfname;

      // 移植label部分
      kpriv->labelptr = tmpxkpriv->labelptr;
      kpriv->labelflags = tmpxkpriv->labelflags;
      kpriv->max_labels =tmpxkpriv->max_labels;


      kpriv->model =   tmpxkpriv->model;

      free(tmpxkpriv);
      //不移植的参数
      // // 色彩，只支持RGB，目前不考虑移植
      // kpriv->modelfmt = tmpxkpriv.modelfmt;

      // // 优先级，目前取消插件内自动切换的功能
      // kpriv->priority = tmpxkpriv.priority;
 
    }

    return 1;
  
}

void fifoComCtrAll(ivas_xkpriv * kpriv){
    cmpk::fifocom *ffc = &kpriv->ffc;
    cmpk::fifoComRead(ffc);
    

    loadDynamicModelfromFFC(kpriv);
    fifoComCtr_DPUInvteral(kpriv);

}
