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

/* 外部变量声明 */
extern UART_HandleTypeDef huart2;   // USART2句柄

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
 * @brief 发送IMU数据 (JSON格式)
 * @return 发送结果 (1=成功, 0=失败)
 */
uint8_t DataSender_SendIMU(void)
{
    char json_buffer[256];
    uint32_t timestamp = HAL_GetTick();
    float acc[3], gyro[3], temp;
    uint8_t mpu_ok = Hardware_MPU6050_ReadSensorData(acc, gyro, &temp);
    
    // 构建JSON格式的IMU数据
    int len = snprintf(json_buffer, sizeof(json_buffer),
        "{"
        "\"type\":\"imu\","
        "\"timestamp\":%lu,"
        "\"data\":{"
            "\"euler\":[0.00,0.00,0.00],"
            "\"acc\":[%.3f,%.3f,%.3f],"
            "\"gyro\":[%.1f,%.1f,%.1f],"
            "\"temp\":%.1f,"
            "\"valid\":%d"
        "}"
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
 * @brief 发送超声波数据 (JSON格式)  
 * @return 发送结果 (1=成功, 0=失败)
 */
uint8_t DataSender_SendUltrasonic(void)
{
    char json_buffer[128];
    uint32_t timestamp = HAL_GetTick();
    float distance = Hardware_Ultrasonic_Measure();
    uint8_t valid = (distance > 0) ? 1 : 0;
    
    // 构建JSON格式的超声波数据
    int len = snprintf(json_buffer, sizeof(json_buffer),
        "{"
        "\"type\":\"ultrasonic\","
        "\"timestamp\":%lu,"
        "\"data\":{"
            "\"distance\":%.1f,"
            "\"valid\":%d"
        "}"
        "}\r\n",
        timestamp,
        distance > 0 ? distance : 0.0f,
        valid
    );
    
    if (len > 0 && len < sizeof(json_buffer)) {
        HAL_StatusTypeDef result = HAL_UART_Transmit(&huart2, (uint8_t*)json_buffer, len, 1000);
        return (result == HAL_OK) ? 1 : 0;
    }
    return 0;
}

/**
 * @brief 发送组合数据包 (JSON格式)
 * @return 发送结果 (1=成功, 0=失败)
 */
uint8_t DataSender_SendCombined(void)
{
    char json_buffer[512];
    uint32_t timestamp = HAL_GetTick();
    
    // 直接从硬件读取传感器数据
    float acc[3], gyro[3], temp;
    float distance = Hardware_Ultrasonic_Measure();
    uint8_t mpu_ok = Hardware_MPU6050_ReadSensorData(acc, gyro, &temp);
    uint8_t ultrasonic_valid = (distance > 0) ? 1 : 0;
    
    g_packet_counter++;
    
    // 构建组合数据包
    int len = snprintf(json_buffer, sizeof(json_buffer),
        "{"
        "\"type\":\"combined\","
        "\"timestamp\":%lu,"
        "\"seq\":%lu,"
        "\"imu\":{"
            "\"euler\":[0.00,0.00,0.00],"
            "\"acc\":[%.3f,%.3f,%.3f],"
            "\"gyro\":[%.1f,%.1f,%.1f],"
            "\"temp\":%.1f,"
            "\"valid\":%d"
        "},"
        "\"ultrasonic\":{"
            "\"distance\":%.1f,"
            "\"valid\":%d"
        "},"
        "\"status\":{"
            "\"robot_mode\":1,"
            "\"battery\":100"
        "}"
        "}\r\n",
        timestamp,
        g_packet_counter,
        mpu_ok ? acc[0] : 0.0f, mpu_ok ? acc[1] : 0.0f, mpu_ok ? acc[2] : 0.0f,  // 加速度 [x, y, z]
        mpu_ok ? gyro[0] : 0.0f, mpu_ok ? gyro[1] : 0.0f, mpu_ok ? gyro[2] : 0.0f, // 角速度 [x, y, z]
        mpu_ok ? temp : 0.0f,          // 温度
        mpu_ok ? 1 : 0,                // IMU数据有效性
        distance > 0 ? distance : 0.0f, // 超声波距离
        ultrasonic_valid               // 超声波数据有效性
    );
    
    if (len > 0 && len < sizeof(json_buffer)) {
        HAL_StatusTypeDef result = HAL_UART_Transmit(&huart2, (uint8_t*)json_buffer, len, 1000);
        return (result == HAL_OK) ? 1 : 0;
    }
    return 0;
}

/**
 * @brief 发送状态数据
 * @param status: 机器人状态码
 * @return 发送结果 (1=成功, 0=失败)
 */
uint8_t DataSender_SendStatus(uint8_t status)
{
    char json_buffer[128];
    uint32_t timestamp = HAL_GetTick();
    
    int len = snprintf(json_buffer, sizeof(json_buffer),
        "{"
        "\"type\":\"status\","
        "\"timestamp\":%lu,"
        "\"data\":{"
            "\"robot_status\":%d,"
            "\"uptime\":%lu"
        "}"
        "}\r\n",
        timestamp,
        status,
        timestamp
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
