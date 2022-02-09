/*
 * @Author: CatMouse
 * @Date: 2022-01-14 21:31:53
 * @LastEditTime: 2022-01-27 20:35:54
 * @LastEditors: CatMouse
 * @Description: 
 * @FilePath: /VVAS/ivas-accel-sw-libs/cm_package/cmpk_platformstats.hpp
 *  Copyright (c) 2021 CatMouse All rights reserved.
 */

#pragma once

#ifndef CMPK_POWER_MONITOR_H
#define CMPK_POWER_MONITOR_H


struct cpustat {
	unsigned long user;
	unsigned long nice;
	unsigned long system;
	unsigned long idle;
	unsigned long iowait;
	unsigned long irq;
	unsigned long softirq;
	double total_util;
};



/************************** Function Prototypes  *****************************/

long cmpk_getSomPowerInfo();
long cmpk_getSomPowerInfo(int hwmonid);
/**
 * @description: 获取温度信息 
 * @param {int} target  可选 0：LPD_TEMP 1：FPD_TEMP 2：PL_TEMP 
 * @param {int} hwmon_id 0:kv260
 * @return {*} 温度*1000
 */
long cmpk_getPlatformTempInfo(int target, int hwmon_id);



void print_all_stats(int verbose_flag);
int print_cpu_utilization(int verbose_flag);
double calculate_load(struct cpustat *prev, struct cpustat *curr);
int print_cpu_stats(struct cpustat *st, int cpu_id);
int get_stats(struct cpustat *st, int cpunum);

int print_ram_memory_utilization(int verbose_flag);
int get_ram_memory_utilization(unsigned long* MemTotal, unsigned long* MemFree, unsigned long* MemAvailable);

int print_swap_memory_utilization(int verbose_flag);
int get_swap_memory_utilization(unsigned long* SwapTotal, unsigned long* SwapFree);

int print_power_utilization(int verbose_flag);
int print_ina260_power_info(int verbose_flag);
int count_hwmon_reg_devices();
int get_device_hwmon_id(int verbose_flag, char* name);
int read_sysfs_entry(char* filename, char* entry);

int print_cma_utilization(int verbose_flag);
int get_cma_utilization(unsigned long* CmaTotal, unsigned long* CmaFree);

int print_cpu_frequency(int verbose_flag);
int get_cpu_frequency(int cpu_id, float* cpu_freq);

#endif