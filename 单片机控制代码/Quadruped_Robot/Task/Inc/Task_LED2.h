/*
 * @Description: CAN通讯头文件声明
 * @Author: Mr Zhang
 * @Github: https://github.com/bigfatpaper
 * @Date: 2019-10-13 14:00:36
 * @LastEditors: Mr Zhang
 * @LastEditTime: 2019-10-16 23:58:33
 */

/*头文件预声明*/
#ifndef __TASKS_CAN_H
#define __TASKS_CAN_H

/*外部变量预声明*/
#ifdef __TASK_CAN_GLOBALS
#define TASK_CAN_EXT
#else
#define TASK_CAN_EXT extern
#endif
/*外部变量预声明结束*/

/*头文件*/
#include "Task_init.h"
/*包含头文件结束*/

/*宏定义*/
#define CANSEND_1 1                        /*CANSEND选择 1 0x200 2 0x1ff*/
#define CANSEND_2 2
/*宏定义结束*/

/*结构体定义*/

typedef struct                             /*CAN发送结构体*/
{
    uint8_t            CANx;               /* CAN1 or CAN2 选择*/
    uint32_t           stdid;              /* ID: CAN1 0X200  CAN2 0X1FF */
		uint8_t            Data[8];            /* CAN发送数据*/
}CanSend_Type;

/*结构体定义结束*/

/*全局变量*/
TASK_CAN_EXT CanSend_Type CAN_Tx_Msg;      /*CAN发送变量声明*/
/*全局变量声明结束*/

/*函数声明*/
/*CAN的初始化函数 此处不启用中断*/
void CAN_Init(CAN_HandleTypeDef *hcan);
void CAN_Recieve(CAN_HandleTypeDef *hcan);

extern int16_t delta_Mechanical_angle(int16_t speed, uint16_t angle_last, uint16_t angle_now);
/*函数声明结束*/

#endif
/*头文件预声明结束*/
