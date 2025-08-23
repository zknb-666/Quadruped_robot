/*
 * @Description: 智能导航数据发送模块头文件
 * @Author: Auto Generated
 * @Date: 2025-08-23
 * @Version: 1.0
 */

#ifndef __DATA_SENDER_H
#define __DATA_SENDER_H

#include "main.h"
#include "usart.h"
#include "MPU6050.h"
#include "Ultrasonic.h"

/* 数据包类型定义 */
typedef enum {
    DATA_TYPE_IMU = 1,          // IMU数据包
    DATA_TYPE_ULTRASONIC = 2,   // 超声波数据包  
    DATA_TYPE_COMBINED = 3,     // 组合数据包
    DATA_TYPE_STATUS = 4        // 状态数据包
} DataPacketType_t;

/* IMU数据结构 */
typedef struct {
    float roll;     // 滚转角 (degrees)
    float pitch;    // 俯仰角 (degrees) 
    float yaw;      // 偏航角 (degrees)
    float acc_x;    // X轴加速度 (m/s²)
    float acc_y;    // Y轴加速度 (m/s²)
    float acc_z;    // Z轴加速度 (m/s²)
    float gyro_x;   // X轴角速度 (°/s)
    float gyro_y;   // Y轴角速度 (°/s)
    float gyro_z;   // Z轴角速度 (°/s)
    uint32_t timestamp; // 时间戳 (ms)
} IMU_DataPacket_t;

/* 超声波数据结构 */
typedef struct {
    float distance;     // 距离 (cm)
    uint8_t valid;      // 数据有效性
    uint32_t timestamp; // 时间戳 (ms)
} Ultrasonic_DataPacket_t;

/* 组合数据包 */
typedef struct {
    IMU_DataPacket_t imu;
    Ultrasonic_DataPacket_t ultrasonic;
    uint8_t robot_status;   // 机器人状态
    uint32_t timestamp;     // 数据包时间戳
} Combined_DataPacket_t;

/* 函数声明 */
void DataSender_Init(void);
uint8_t DataSender_SendIMU(void);
uint8_t DataSender_SendUltrasonic(void);
uint8_t DataSender_SendCombined(void);
uint8_t DataSender_SendStatus(uint8_t status);

/* 数据发送任务 */
void Task_DataSender_Start(void *parameters);

#endif /* __DATA_SENDER_H */
