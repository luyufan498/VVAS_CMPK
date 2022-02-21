/*
 * @Author: CatMouse
 * @Date: 2022-01-17 22:02:26
 * @LastEditTime: 2022-01-18 02:04:48
 * @LastEditors: CatMouse
 * @Description: 
 * @FilePath: /VVAS/ivas-accel-sw-libs/ivas_xdpuinfer/src/dpuinfer_partial_model.hpp
 *  Copyright (c) 2021 CatMouse All rights reserved.
 */
#pragma once

#ifndef DPU_PARTIAL_MODEL_H
#define DPU_PARTIAL_MODEL_H

#include "ivas_xdpupriv.hpp"

long long get_time ();
std::string modelexits (ivas_xkpriv * kpriv);
labels *readlabel (ivas_xkpriv * kpriv, char *json_file);
void ivas_clean_currentmodel(ivas_xkpriv * kpriv);
int ivas_xclass_to_num (char *name);

IVASVideoFormat ivas_fmt_to_xfmt (char *name);
ivas_xdpumodel *ivas_xinitmodel (ivas_xkpriv * kpriv, int modelclass);
int ivas_xrunmodel (ivas_xkpriv * kpriv, cv::Mat & image,GstInferenceMeta * infer_meta, IVASFrame * inframe);
int ivas_xsetcaps (ivas_xkpriv * kpriv, ivas_xdpumodel * model);

#endif
