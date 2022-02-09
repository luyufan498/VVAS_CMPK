/*
 * @Author: CatMouse
 * @Date: 2022-01-07 16:25:10
 * @LastEditTime: 2022-01-17 22:49:41
 * @LastEditors: CatMouse
 * @Description: 
 * @FilePath: /VVAS/ivas-accel-sw-libs/cm_package/cmpk_json_utils.hpp
 *  Copyright (c) 2021 CatMouse All rights reserved.
 */
#pragma once

#ifndef CMPK_JSON_UTILS_H
#define CMPK_JSON_UTILS_H

#include <cstring>
#include <string>
#include <fstream>
#include <ivas/ivas_kernel.h>
#include <ivas/ivaslogs.h>


using namespace std;




// C语言用不了模板函数
// 假模板 
int JsonGetT(json_t *jconfig, int *dataptr, char const* targetlabel, int default_data, int log_level);
int JsonGetT(json_t *jconfig, uint *dataptr, char const* targetlabel, int default_data, int log_level);
int JsonGetT(json_t *jconfig, float *dataptr, char const* targetlabel, float default_data, int log_level);
int JsonGetT(json_t *jconfig, bool *dataptr, char const* targetlabel, bool default_data, int log_level);


int JsonGet_int(json_t *jconfig, int *dataptr, char const* targetlabel, int default_data, int log_level);
int JsonGet_int(json_t *jconfig, uint *dataptr, char const* targetlabel, int default_data, int log_level);
int JsonGet_float(json_t *jconfig, float *dataptr, char const* targetlabel, float default_data, int log_level);
int JsonGet_bool(json_t *jconfig, bool *dataptr, char const* targetlabel, bool default_data, int log_level);
int JsonGet_rgb(json_t *jconfig, uint *red, uint *green, uint *blue, char const* targetlabel, uint default_rgb, int log_level);

int JsonGet_string(json_t *jconfig, string *dataptr, char const* targetlabel, char const* default_data, int log_level);





// 读取 int 值
int XkprivGetJsonData_int(json_t *jconfig, int *dataptr, char const* targetlabel, int default_data, int log_level);
int XkprivGetJsonData_int(json_t *jconfig, uint *dataptr, char const* targetlabel, int default_data, int log_level);
// 读取 Uint 值
int XkprivGetJsonData_uint(json_t *jconfig, uint *dataptr, char const* targetlabel, uint default_data, int log_level);
int XkprivGetJsonData_float(json_t *jconfig, float *dataptr, char const* targetlabel, float default_data, int log_level);



// 读取 bool 值
int XkprivGetJsonData_bool(json_t *jconfig, bool *dataptr, char const* targetlabel, bool default_data, int log_level);
typedef int (*xkprivStringProcessAPI)(char *);  
// 读取字符串 后交给 api_string_to_num 处理，最终转换成 Int
int XkprivGetJsonData_string2Int(json_t *jconfig, int *dataptr, char const* targetlabel, xkprivStringProcessAPI api_string_to_num, int default_data, int log_level);
// 读取字符串到 string
int XkprivGetJsonData_string(json_t *jconfig, string *dataptr, char const* targetlabel, char const* default_data, int log_level);
// 读取到字符串到 char[]
// 无长度限制，可能溢出
int XkprivGetJsonData_chars(json_t *jconfig, char *dataptr, char const* targetlabel, char const* default_data, int log_level);

#endif
