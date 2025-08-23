/*
 * @Description: 调试输出重定向实现
 * @Author: Auto Generated
 * @Date: 2025-08-23
 */

#include "Debug_Output.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

/* 全局变量 */
DebugLevel_t g_debug_level = DEBUG_LEVEL_INFO;

/* 私有变量 */
static char debug_buffer[512];  // 调试信息缓冲区

/**
 * @brief 调试模块初始化
 */
void Debug_Init(void)
{
    g_debug_level = DEBUG_LEVEL_INFO;
    
    // 发送初始化信息到蓝牙
    const char *init_msg = "\r\n=== Debug Output Redirected to Bluetooth (USART2) ===\r\n";
    HAL_UART_Transmit(DEBUG_UART_HANDLE, (uint8_t*)init_msg, strlen(init_msg), DEBUG_TIMEOUT);
}

/**
 * @brief 设置调试级别
 * @param level: 调试级别
 */
void Debug_SetLevel(DebugLevel_t level)
{
    g_debug_level = level;
    
    snprintf(debug_buffer, sizeof(debug_buffer), "Debug level set to: %d\r\n", level);
    HAL_UART_Transmit(DEBUG_UART_HANDLE, (uint8_t*)debug_buffer, strlen(debug_buffer), DEBUG_TIMEOUT);
}

/**
 * @brief 格式化调试输出
 * @param format: 格式化字符串
 * @param ...: 可变参数
 * @return 发送的字符数
 */
int Debug_Printf(const char *format, ...)
{
    va_list args;
    int len;
    
    va_start(args, format);
    len = vsnprintf(debug_buffer, sizeof(debug_buffer), format, args);
    va_end(args);
    
    if (len > 0 && len < sizeof(debug_buffer)) {
        HAL_UART_Transmit(DEBUG_UART_HANDLE, (uint8_t*)debug_buffer, len, DEBUG_TIMEOUT);
        return len;
    }
    
    return 0;
}

/**
 * @brief 带级别的格式化调试输出
 * @param level: 调试级别
 * @param format: 格式化字符串
 * @param ...: 可变参数
 * @return 发送的字符数
 */
int Debug_PrintfLevel(DebugLevel_t level, const char *format, ...)
{
    if (level > g_debug_level) {
        return 0;  // 级别不够，不输出
    }
    
    va_list args;
    int len;
    const char *level_str[] = {"[ERROR]", "[WARN] ", "[INFO] ", "[DEBUG]"};
    
    // 添加级别标识
    len = snprintf(debug_buffer, sizeof(debug_buffer), "%s ", level_str[level]);
    
    // 添加用户格式化内容
    va_start(args, format);
    len += vsnprintf(debug_buffer + len, sizeof(debug_buffer) - len, format, args);
    va_end(args);
    
    if (len > 0 && len < sizeof(debug_buffer)) {
        HAL_UART_Transmit(DEBUG_UART_HANDLE, (uint8_t*)debug_buffer, len, DEBUG_TIMEOUT);
        return len;
    }
    
    return 0;
}

/**
 * @brief 重定向fputc到USART2
 * @param ch: 要发送的字符
 * @param f: 文件指针(未使用)
 * @return 发送的字符
 */
#ifdef __GNUC__
int __io_putchar(int ch)
#else
int fputc(int ch, FILE *f)
#endif
{
    HAL_UART_Transmit(DEBUG_UART_HANDLE, (uint8_t *)&ch, 1, DEBUG_TIMEOUT);
    return ch;
}
