/*
 * @Description: 智能导航数据发送模块实现
 * @Author: Auto Generated  
 * @Date: 2025-08-23
 * @Version: 1.0
 */

#include "Data_Sender.h"
#include "stdio.h"
#include "string.h"
#include "cmsis_os.h"
#include "usart.h"
#include "Debug_Output.h"
#include "Hardware_Config.h"  // 使用硬件配置函数

/* 私有变量 */
static Combined_DataPacket_t g_combined_data;
static uint32_t g_packet_counter = 0;

/* 私有函数声明 */
static const char* GetGaitModeString(GaitMode_t mode);
static const char* GetMainStateString(MainState_t state);

/* 外部变量声明 */
extern UART_HandleTypeDef huart2;   // USART2句柄

/**
 * @brief 获取步态模式字符串
 * @param mode: 步态模式枚举
 * @return 模式字符串
 */
static const char* GetGaitModeString(GaitMode_t mode)
{
    switch(mode) {
        case GaitMode_Stand:       return "stand";
        case GaitMode_Walk:        return "walk";
        case GaitMode_Trot:        return "trot";
        case GaitMode_Crawl:       return "crawl";
        case GaitMode_Body_Twist:  return "body_twist";
        default:                   return "unknown";
    }
}

/**
 * @brief 获取主状态字符串
 * @param state: 主状态枚举
 * @return 状态字符串
 */
static const char* GetMainStateString(MainState_t state)
{
    switch(state) {
        case MainState_Static:         return "static";
        case MainState_Step:           return "step";
        case MainState_Forward:        return "forward";
        case MainState_Backward:       return "backward";
        case MainState_Move_to_Left:   return "move_left";
        case MainState_Move_to_Right:  return "move_right";
        case MainState_Turn_Left:      return "turn_left";
        case MainState_Turn_Right:     return "turn_right";
        case MainState_IMU:            return "imu_stabilize";
        case MainState_Avoid_Obstacle: return "avoid_obstacle";
        default:                       return "unknown";
    }
}

/**
 * @brief 数据发送模块初始化
 */
void DataSender_Init(void)
{
    memset(&g_combined_data, 0, sizeof(Combined_DataPacket_t));
    g_packet_counter = 0;
    
    // 简化初始化信息
    HAL_UART_Transmit(&huart2, (uint8_t*)"[INIT] Data Sender OK\r\n", 23, 1000);
}

/**
 * @brief 发送IMU数据 (简化JSON格式)
 * @return 发送结果 (1=成功, 0=失败)
 */
uint8_t DataSender_SendIMU(void)
{
    char json_buffer[192];
    uint32_t timestamp = HAL_GetTick();
    float acc[3], gyro[3], temp;
    uint8_t mpu_ok = Hardware_MPU6050_ReadSensorData(acc, gyro, &temp);
    
    // 简化JSON格式 - 扁平化结构
    int len = snprintf(json_buffer, sizeof(json_buffer),
        "{"
        "\"t\":%lu,"
        "\"ax\":%.3f,\"ay\":%.3f,\"az\":%.3f,"
        "\"gx\":%.1f,\"gy\":%.1f,\"gz\":%.1f,"
        "\"temp\":%.1f,"
        "\"ok\":%d"
        "}\r\n",
        timestamp,
        mpu_ok ? acc[0] : 0.0f, mpu_ok ? acc[1] : 0.0f, mpu_ok ? acc[2] : 0.0f,
        mpu_ok ? gyro[0] : 0.0f, mpu_ok ? gyro[1] : 0.0f, mpu_ok ? gyro[2] : 0.0f,
        mpu_ok ? temp : 0.0f,
        mpu_ok ? 1 : 0
    );
    
    if (len > 0 && len < sizeof(json_buffer)) {
        HAL_StatusTypeDef result = HAL_UART_Transmit(&huart2, (uint8_t*)json_buffer, len, 1000);
        return (result == HAL_OK) ? 1 : 0;
    }
    return 0;
}

/**
 * @brief 发送超声波数据 (简化JSON格式)  
 * @return 发送结果 (1=成功, 0=失败)
 */
uint8_t DataSender_SendUltrasonic(void)
{
    char json_buffer[96];
    uint32_t timestamp = HAL_GetTick();
    float distance = Hardware_Ultrasonic_Measure();
    
    // 简化JSON格式
    int len = snprintf(json_buffer, sizeof(json_buffer),
        "{"
        "\"t\":%lu,"
        "\"dist\":%.1f,"
        "\"ok\":%d"
        "}\r\n",
        timestamp,
        distance > 0 ? distance : 0.0f,
        (distance > 0) ? 1 : 0
    );
    
    if (len > 0 && len < sizeof(json_buffer)) {
        HAL_StatusTypeDef result = HAL_UART_Transmit(&huart2, (uint8_t*)json_buffer, len, 1000);
        return (result == HAL_OK) ? 1 : 0;
    }
    return 0;
}

/**
 * @brief 发送组合数据包 (简化JSON格式)
 * @return 发送结果 (1=成功, 0=失败)
 */
uint8_t DataSender_SendCombined(void)
{
    char json_buffer[256];
    uint32_t timestamp = HAL_GetTick();
    
    // 直接从硬件读取传感器数据
    float acc[3], gyro[3], temp;
    float distance = Hardware_Ultrasonic_Measure();
    uint8_t mpu_ok = Hardware_MPU6050_ReadSensorData(acc, gyro, &temp);
    
    // 获取当前模式信息
    GaitMode_t current_gait_mode = GetGaitMode();
    MainState_t current_main_state = GetMainState();
    
    g_packet_counter++;
    
    // 简化的组合数据包 - 扁平化结构，使用短键名
    int len = snprintf(json_buffer, sizeof(json_buffer),
        "{"
        "\"t\":%lu,"
        "\"seq\":%lu,"
        "\"ax\":%.3f,\"ay\":%.3f,\"az\":%.3f,"
        "\"gx\":%.1f,\"gy\":%.1f,\"gz\":%.1f,"
        "\"temp\":%.1f,"
        "\"imu_ok\":%d,"
        "\"dist\":%.1f,"
        "\"dist_ok\":%d,"
        "\"mode\":%d,"
        "\"state\":%d"
        "}\r\n",
        timestamp,
        g_packet_counter,
        mpu_ok ? acc[0] : 0.0f, mpu_ok ? acc[1] : 0.0f, mpu_ok ? acc[2] : 0.0f,
        mpu_ok ? gyro[0] : 0.0f, mpu_ok ? gyro[1] : 0.0f, mpu_ok ? gyro[2] : 0.0f,
        mpu_ok ? temp : 0.0f,
        mpu_ok ? 1 : 0,
        distance > 0 ? distance : 0.0f,
        (distance > 0) ? 1 : 0,
        (int)current_gait_mode,
        (int)current_main_state
    );
    
    if (len > 0 && len < sizeof(json_buffer)) {
        HAL_StatusTypeDef result = HAL_UART_Transmit(&huart2, (uint8_t*)json_buffer, len, 1000);
        return (result == HAL_OK) ? 1 : 0;
    }
    return 0;
}

/**
 * @brief 发送状态数据 (简化JSON格式)
 * @param status: 机器人状态码
 * @return 发送结果 (1=成功, 0=失败)
 */
uint8_t DataSender_SendStatus(uint8_t status)
{
    char json_buffer[128];
    uint32_t timestamp = HAL_GetTick();
    
    // 获取当前模式信息
    GaitMode_t current_gait_mode = GetGaitMode();
    MainState_t current_main_state = GetMainState();
    
    // 简化JSON格式
    int len = snprintf(json_buffer, sizeof(json_buffer),
        "{"
        "\"t\":%lu,"
        "\"status\":%d,"
        "\"uptime\":%lu,"
        "\"mode\":%d,"
        "\"state\":%d"
        "}\r\n",
        timestamp,
        status,
        timestamp,
        (int)current_gait_mode,
        (int)current_main_state
    );
    
    if (len > 0 && len < sizeof(json_buffer)) {
        HAL_StatusTypeDef result = HAL_UART_Transmit(&huart2, (uint8_t*)json_buffer, len, 1000);
        return (result == HAL_OK) ? 1 : 0;
    }
    return 0;
}

/**
 * @brief 数据发送任务
 * @param parameters: 任务参数
 */
void Task_DataSender_Start(void *parameters)
{
    // 简化启动信息
    HAL_UART_Transmit(&huart2, (uint8_t*)"[DATA] Start\r\n", 14, 1000);
    
    while(1)
    {
        // 每500ms发送一次JSON数据（减少频率）
        DataSender_SendCombined();
        
        // 延时500ms
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
