/*
 * @Description: 超声波模块头文件
 * @Author: Auto Generated
 * @Date: 2025-08-23
 */

#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H

#include "main.h"

/* 超声波数据结构 */
typedef struct {
    float distance;     // 距离值 (cm)
    uint8_t valid;      // 数据有效标志
    uint32_t timestamp; // 时间戳
} Ultrasonic_Data_t;

/* 函数声明 */
void Ultrasonic_Init(void);
uint8_t Ultrasonic_ProcessData(uint8_t *data, uint8_t length);
float Ultrasonic_GetDistance(void);
uint8_t Ultrasonic_IsDataValid(void);

#endif /* __ULTRASONIC_H */
