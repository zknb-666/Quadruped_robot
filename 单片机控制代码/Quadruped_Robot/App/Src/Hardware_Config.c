/*
 * @Description: 硬件配置修复实现 - 保持现有接线
 * @Author: Auto Generated
 * @Date: 2025-08-23
 */

#include "Hardware_Config.h"
#include "gpio.h"
#include "Debug_Output.h"

/* ==================== 软件I2C实现 ==================== */

/* 软件I2C延时函数 */
static void I2C_Delay(void)
{
    volatile uint32_t i = 10; // 调整延时适应I2C时序
    while(i--);
}

/* SCL引脚控制 */
static void I2C_SCL_High(void)
{
    HAL_GPIO_WritePin(MPU6050_SCL_PORT, MPU6050_SCL_PIN, GPIO_PIN_SET);
}

static void I2C_SCL_Low(void)
{
    HAL_GPIO_WritePin(MPU6050_SCL_PORT, MPU6050_SCL_PIN, GPIO_PIN_RESET);
}

/* SDA引脚控制 */
static void I2C_SDA_High(void)
{
    HAL_GPIO_WritePin(MPU6050_SDA_PORT, MPU6050_SDA_PIN, GPIO_PIN_SET);
}

static void I2C_SDA_Low(void)
{
    HAL_GPIO_WritePin(MPU6050_SDA_PORT, MPU6050_SDA_PIN, GPIO_PIN_RESET);
}

/* 读取SDA状态 */
static uint8_t I2C_SDA_Read(void)
{
    return HAL_GPIO_ReadPin(MPU6050_SDA_PORT, MPU6050_SDA_PIN);
}

/* 重新配置GPIO为I2C功能 */
static void Hardware_Config_Fix_GPIO_For_I2C(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /* 启用GPIOB时钟 */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    /* 配置PB10为SCL (开漏输出) */
    GPIO_InitStruct.Pin = MPU6050_SCL_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(MPU6050_SCL_PORT, &GPIO_InitStruct);
    
    /* 配置PB11为SDA (开漏输出) */
    GPIO_InitStruct.Pin = MPU6050_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(MPU6050_SDA_PORT, &GPIO_InitStruct);
    
    /* 初始状态设为高电平 */
    I2C_SCL_High();
    I2C_SDA_High();
}

/* 重新配置GPIO为超声波功能 */
static void Hardware_Config_Fix_GPIO_For_Ultrasonic(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /* 启用GPIOC时钟 */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    /* 配置PC11为TRIG (推挽输出) */
    GPIO_InitStruct.Pin = ULTRASONIC_TRIG_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ULTRASONIC_TRIG_PORT, &GPIO_InitStruct);
    
    /* 配置PC10为ECHO (浮空输入) */
    GPIO_InitStruct.Pin = ULTRASONIC_ECHO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(ULTRASONIC_ECHO_PORT, &GPIO_InitStruct);
    
    /* TRIG初始状态为低电平 */
    HAL_GPIO_WritePin(ULTRASONIC_TRIG_PORT, ULTRASONIC_TRIG_PIN, GPIO_PIN_RESET);
}

/* ==================== 超声波控制函数 ==================== */

/* 超声波测距函数 */
float Hardware_Ultrasonic_Measure(void)
{
    uint32_t echo_time = 0;
    uint32_t start_time, end_time;
    float distance;
    
    /* 发送触发脉冲 - 需要10us以上 */
    HAL_GPIO_WritePin(ULTRASONIC_TRIG_PORT, ULTRASONIC_TRIG_PIN, GPIO_PIN_SET);
    // 使用更精确的延时
    for(volatile int i = 0; i < 720; i++); // 大约10us @72MHz
    HAL_GPIO_WritePin(ULTRASONIC_TRIG_PORT, ULTRASONIC_TRIG_PIN, GPIO_PIN_RESET);
    
    /* 等待ECHO信号变高 */
    uint32_t timeout = HAL_GetTick() + 100; // 100ms超时
    while (HAL_GPIO_ReadPin(ULTRASONIC_ECHO_PORT, ULTRASONIC_ECHO_PIN) == GPIO_PIN_RESET) {
        if (HAL_GetTick() > timeout) {
            return -1.0f;  // 简化错误处理
        }
    }
    start_time = HAL_GetTick();
    
    /* 等待ECHO信号变低 */
    timeout = HAL_GetTick() + 200; // 200ms超时
    while (HAL_GPIO_ReadPin(ULTRASONIC_ECHO_PORT, ULTRASONIC_ECHO_PIN) == GPIO_PIN_SET) {
        if (HAL_GetTick() > timeout) {
            return -1.0f;  // 简化错误处理
        }
    }
    end_time = HAL_GetTick();
    
    /* 计算距离 */
    echo_time = end_time - start_time;
    distance = (echo_time * 1000.0f * 0.034f) / 2.0f; // 转换为厘米
    
    return distance;
}

/* ==================== 软件I2C通信函数 ==================== */

/* I2C起始信号 */
static void I2C_Start(void)
{
    I2C_SDA_High();
    I2C_SCL_High();
    I2C_Delay();
    I2C_SDA_Low();
    I2C_Delay();
    I2C_SCL_Low();
    I2C_Delay();
}

/* I2C停止信号 */
static void I2C_Stop(void)
{
    I2C_SDA_Low();
    I2C_SCL_High();
    I2C_Delay();
    I2C_SDA_High();
    I2C_Delay();
}

/* I2C发送一个字节 */
static uint8_t I2C_SendByte(uint8_t data)
{
    uint8_t i;
    
    for (i = 0; i < 8; i++) {
        if (data & 0x80) {
            I2C_SDA_High();
        } else {
            I2C_SDA_Low();
        }
        I2C_Delay();
        I2C_SCL_High();
        I2C_Delay();
        I2C_SCL_Low();
        I2C_Delay();
        data <<= 1;
    }
    
    /* 等待ACK */
    I2C_SDA_High();
    I2C_Delay();
    I2C_SCL_High();
    I2C_Delay();
    uint8_t ack = I2C_SDA_Read();
    I2C_SCL_Low();
    I2C_Delay();
    
    return ack;
}

/* I2C接收一个字节 */
static uint8_t I2C_ReceiveByte(uint8_t ack)
{
    uint8_t data = 0;
    uint8_t i;
    
    I2C_SDA_High();
    for (i = 0; i < 8; i++) {
        data <<= 1;
        I2C_SCL_High();
        I2C_Delay();
        if (I2C_SDA_Read()) {
            data |= 0x01;
        }
        I2C_SCL_Low();
        I2C_Delay();
    }
    
    /* 发送ACK/NACK */
    if (ack) {
        I2C_SDA_Low();
    } else {
        I2C_SDA_High();
    }
    I2C_Delay();
    I2C_SCL_High();
    I2C_Delay();
    I2C_SCL_Low();
    I2C_Delay();
    I2C_SDA_High();
    
    return data;
}

/* MPU6050读取寄存器 */
uint8_t Hardware_MPU6050_ReadReg(uint8_t reg)
{
    uint8_t data;
    
    I2C_Start();
    I2C_SendByte(0xD0); // MPU6050写地址
    I2C_SendByte(reg);
    
    I2C_Start();
    I2C_SendByte(0xD1); // MPU6050读地址
    data = I2C_ReceiveByte(0);
    I2C_Stop();
    
    return data;
}

/* MPU6050写入寄存器 */
void Hardware_MPU6050_WriteReg(uint8_t reg, uint8_t data)
{
    I2C_Start();
    I2C_SendByte(0xD0); // MPU6050写地址
    I2C_SendByte(reg);
    I2C_SendByte(data);
    I2C_Stop();
}

/* MPU6050初始化 */
uint8_t Hardware_MPU6050_Init(void)
{
    uint8_t who_am_i;
    
    /* 读取WHO_AM_I寄存器验证连接 */
    who_am_i = Hardware_MPU6050_ReadReg(0x75);
    if (who_am_i != 0x68) {
        return 0;  // 简化错误处理
    }
    
    /* 基本配置 */
    Hardware_MPU6050_WriteReg(0x6B, 0x00); // 唤醒设备
    HAL_Delay(100);
    Hardware_MPU6050_WriteReg(0x19, 0x07); // 设置采样率
    Hardware_MPU6050_WriteReg(0x1A, 0x06); // 配置DLPF
    Hardware_MPU6050_WriteReg(0x1B, 0x18); // 陀螺仪满量程±2000°/s
    Hardware_MPU6050_WriteReg(0x1C, 0x01); // 加速度计满量程±2g
    
    return 1;
}

/* MPU6050读取传感器数据 */
uint8_t Hardware_MPU6050_ReadSensorData(float *acc, float *gyro, float *temp)
{
    uint8_t data[14];
    int16_t raw_data[7];
    
    // 从寄存器0x3B开始连续读取14个字节
    for (int i = 0; i < 14; i++) {
        data[i] = Hardware_MPU6050_ReadReg(0x3B + i);
    }
    
    // 组合原始数据
    raw_data[0] = (int16_t)((data[0] << 8) | data[1]);   // ACCEL_X
    raw_data[1] = (int16_t)((data[2] << 8) | data[3]);   // ACCEL_Y  
    raw_data[2] = (int16_t)((data[4] << 8) | data[5]);   // ACCEL_Z
    raw_data[3] = (int16_t)((data[6] << 8) | data[7]);   // TEMP
    raw_data[4] = (int16_t)((data[8] << 8) | data[9]);   // GYRO_X
    raw_data[5] = (int16_t)((data[10] << 8) | data[11]); // GYRO_Y
    raw_data[6] = (int16_t)((data[12] << 8) | data[13]); // GYRO_Z
    
    // 转换为实际物理量
    if (acc != NULL) {
        acc[0] = raw_data[0] / 16384.0f * 9.8f;  // ±2g -> m/s²
        acc[1] = raw_data[1] / 16384.0f * 9.8f;
        acc[2] = raw_data[2] / 16384.0f * 9.8f;
    }
    
    if (gyro != NULL) {
        gyro[0] = raw_data[4] / 131.0f;  // ±250°/s -> °/s
        gyro[1] = raw_data[5] / 131.0f;
        gyro[2] = raw_data[6] / 131.0f;
    }
    
    if (temp != NULL) {
        *temp = raw_data[3] / 340.0f + 36.53f;  // 温度
    }
    
    return 1;
}

/* ==================== 主要功能函数 ==================== */

void Hardware_Config_Init(void)
{
    /* 重新配置GPIO */
    Hardware_Config_Fix_GPIO_For_I2C();
    Hardware_Config_Fix_GPIO_For_Ultrasonic();
    
    /* 初始化MPU6050 */
    Hardware_MPU6050_Init();
}

void Hardware_Config_Print_Status(void)
{
    // 移除详细状态输出，保持静默
}

/* 提供给外部调用的测试函数 */
void Hardware_Test_MPU6050(void)
{
    uint8_t who_am_i = Hardware_MPU6050_ReadReg(0x75);
    if (who_am_i == 0x68) {
        float acc[3], gyro[3], temp;
        Hardware_MPU6050_ReadSensorData(acc, gyro, &temp);
        // 只在测试时输出简化信息
        DEBUG_INFO("MPU6050 OK - ACC(%.1f,%.1f,%.1f) GYRO(%.1f,%.1f,%.1f) T=%.1f\r\n",
                   acc[0], acc[1], acc[2], gyro[0], gyro[1], gyro[2], temp);
    }
}

void Hardware_Test_Ultrasonic(void)
{
    float distance = Hardware_Ultrasonic_Measure();
    if (distance > 0) {
        DEBUG_INFO("Ultrasonic: %.1f cm\r\n", distance);
    }
}
