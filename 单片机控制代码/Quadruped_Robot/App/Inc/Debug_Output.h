/*
 * @Description: 调试输出重定向头文件
 * @Author: Auto Generated
 * @Date: 2025-08-23
 */

#ifndef __DEBUG_OUTPUT_H
#define __DEBUG_OUTPUT_H

#include "main.h"
#include "usart.h"

/* 调试输出配置 */
#define DEBUG_UART_HANDLE    (&huart2)    // 使用USART2作为调试输出
#define DEBUG_TIMEOUT        1000         // 发送超时时间(ms)

/* 调试级别定义 */
typedef enum {
    DEBUG_LEVEL_ERROR = 0,
    DEBUG_LEVEL_WARN  = 1,
    DEBUG_LEVEL_INFO  = 2,
    DEBUG_LEVEL_DEBUG = 3
} DebugLevel_t;

/* 当前调试级别 */
extern DebugLevel_t g_debug_level;

/* 函数声明 */
void Debug_Init(void);
void Debug_SetLevel(DebugLevel_t level);
int Debug_Printf(const char *format, ...);
int Debug_PrintfLevel(DebugLevel_t level, const char *format, ...);

/* 调试宏定义 */
#define DEBUG_ERROR(...)   Debug_PrintfLevel(DEBUG_LEVEL_ERROR, __VA_ARGS__)
#define DEBUG_WARN(...)    Debug_PrintfLevel(DEBUG_LEVEL_WARN,  __VA_ARGS__)
#define DEBUG_INFO(...)    Debug_PrintfLevel(DEBUG_LEVEL_INFO,  __VA_ARGS__)
#define DEBUG_DEBUG(...)   Debug_PrintfLevel(DEBUG_LEVEL_DEBUG, __VA_ARGS__)

/* 兼容性宏 - 将原有的printf重定向到蓝牙 */
#define printf(...) Debug_Printf(__VA_ARGS__)

#endif /* __DEBUG_OUTPUT_H */
