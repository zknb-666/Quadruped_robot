/*
 * @Description: 超声波模块数据处理
 * @Author: Auto Generated  
 * @Date: 2025-08-23
 */

#include "Ultrasonic.h"
#include "stdio.h"
#include "Debug_Output.h"  // 添加调试输出重定向

/* 全局变量 */
static Ultrasonic_Data_t ultrasonic_data = {0};
extern uint8_t Re_buf2[10];

/**
 * @brief 超声波模块初始化
 */
void Ultrasonic_Init(void)
{
    ultrasonic_data.distance = 0.0f;
    ultrasonic_data.valid = 0;
    ultrasonic_data.timestamp = 0;
    
    DEBUG_INFO("Ultrasonic module initialized\r\n");
}

/**
 * @brief 处理超声波数据
 * @param data: 原始数据缓冲区
 * @param length: 数据长度
 * @return 处理结果 (1=成功, 0=失败)
 */
uint8_t Ultrasonic_ProcessData(uint8_t *data, uint8_t length)
{
    if (data == NULL || length < 5) {
        return 0;
    }
    
    // 检查帧头 (假设超声波模块帧头为0x44, 即68)
    if (data[0] != 68) {
        DEBUG_WARN("Ultrasonic: Invalid frame header 0x%02X\r\n", data[0]);
        return 0;
    }
    
    // 解析距离数据 (根据具体超声波模块协议调整)
    // 假设数据格式: [帧头][距离高字节][距离低字节][校验][帧尾]
    uint16_t distance_raw = (data[1] << 8) | data[2];
    
    // 校验数据有效性
    if (distance_raw > 0 && distance_raw < 4000) { // 假设有效范围0-40m
        ultrasonic_data.distance = distance_raw / 10.0f; // 转换为cm
        ultrasonic_data.valid = 1;
        ultrasonic_data.timestamp = HAL_GetTick();
        
        DEBUG_INFO("Ultrasonic Distance: %.1f cm\r\n", ultrasonic_data.distance);
        return 1;
    } else {
        ultrasonic_data.valid = 0;
        DEBUG_WARN("Ultrasonic: Invalid distance data %d\r\n", distance_raw);
        return 0;
    }
}

/**
 * @brief 获取当前距离值
 * @return 距离值 (cm)
 */
float Ultrasonic_GetDistance(void)
{
    return ultrasonic_data.distance;
}

/**
 * @brief 检查数据有效性
 * @return 1=有效, 0=无效
 */
uint8_t Ultrasonic_IsDataValid(void)
{
    // 检查数据时效性 (超过1秒认为无效)
    if (ultrasonic_data.valid && 
        (HAL_GetTick() - ultrasonic_data.timestamp) < 1000) {
        return 1;
    }
    return 0;
}
