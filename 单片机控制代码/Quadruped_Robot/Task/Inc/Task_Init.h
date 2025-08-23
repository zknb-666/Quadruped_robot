#ifndef __TASKS_INIT_H__
#define __TASKS_INIT_H__

#include "sysconfig.h"
#include "stdio.h"
#include "usart.h"

#ifdef 	__TASK_INIT_GLOBALS
#define TASK_INIT_EXT
#else
#define TASK_INIT_EXT extern
#endif
TASK_INIT_EXT TaskHandle_t TaskHandle_IMU;
TASK_INIT_EXT TaskHandle_t TaskHandle_DataSender;  // 添加数据发送任务句柄

void Task_IMU(void *parameters);
void Task_DataSender_Start(void *parameters);  // 添加数据发送任务声明

#endif
