/*
 * @Description: 硬件接线配置说明
 * @Author: Auto Generated
 * @Date: 2025-08-23
 */

#ifndef __HARDWARE_CONFIG_H
#define __HARDWARE_CONFIG_H

#include "main.h"

/* ==================== 硬件接线配置说明 ==================== */

/**
 * 当前系统配置问题分析:
 * 
 * 1. 陀螺仪接线问题:
 *    - 用户接线: SCL->PB10, SDA->PB11
 *    - 系统配置: PB10/PB11 配置为 USART3_TX/RX
 *    - 问题: 引脚被配置为串口而非I2C，导致I2C通信无法正常工作
 * 
 * 2. 超声波模块接线问题:
 *    - 用户接线: TRIG->PC11, ECHO->PC10
 *    - 系统配置: PC10/PC11 配置为 UART4_TX/RX
 *    - 问题: 引脚被配置为串口而非GPIO，无法进行超声波测距
 */

/* ==================== 解决方案配置 ==================== */

/* 保持现有接线，修改软件配置 */

/* 陀螺仪 - 使用软件I2C */
#define MPU6050_SCL_PORT        GPIOB
#define MPU6050_SCL_PIN         GPIO_PIN_10
#define MPU6050_SDA_PORT        GPIOB
#define MPU6050_SDA_PIN         GPIO_PIN_11

/* 超声波 - 重新配置PC10/PC11为GPIO */
#define ULTRASONIC_ECHO_PORT    GPIOC
#define ULTRASONIC_ECHO_PIN     GPIO_PIN_10
#define ULTRASONIC_TRIG_PORT    GPIOC
#define ULTRASONIC_TRIG_PIN     GPIO_PIN_11

/* ==================== 函数声明 ==================== */
void Hardware_Config_Init(void);
void Hardware_Config_Print_Status(void);
void Hardware_Test_MPU6050(void);
void Hardware_Test_Ultrasonic(void);
uint8_t Hardware_MPU6050_ReadReg(uint8_t reg);
void Hardware_MPU6050_WriteReg(uint8_t reg, uint8_t data);
uint8_t Hardware_MPU6050_ReadSensorData(float *acc, float *gyro, float *temp);
float Hardware_Ultrasonic_Measure(void);

#endif /* __HARDWARE_CONFIG_H */
