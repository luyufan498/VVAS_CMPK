/*
 * @Author: CatMouse
 * @Date: 2022-01-17 21:52:41
 * @LastEditTime: 2022-01-18 02:11:51
 * @LastEditors: CatMouse
 * @Description: 
 * @FilePath: /VVAS/ivas-accel-sw-libs/ivas_xdpuinfer/src/dpuinfer_partial_ffc.hpp
 *  Copyright (c) 2021 CatMouse All rights reserved.
 */

#pragma once

#ifndef DPU_PARTIAL_FFC_H
#define DPU_PARTIAL_FFC_H

#include "ivas_xdpupriv.hpp"


using namespace cv;
using namespace std;
using namespace cmpk;

/**
 * @description: 所有的通讯代码都放到这里来进行处理简化代码
 * @param {ivas_xkpriv *} kpriv
 * @return {*}
 */

void fifoComCtrAll(ivas_xkpriv * kpriv);


#endif