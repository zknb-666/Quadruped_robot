/*
 * @Description: 调试系统测试函数
 * @Author: Auto Generated
 * @Date: 2025-08-23
 */

#include "Debug_Output.h"
#include "Task_init.h"

/**
 * @brief 测试调试输出系统
 */
void Debug_Test(void)
{
    DEBUG_ERROR("This is an ERROR message test\r\n");
    DEBUG_WARN("This is a WARNING message test\r\n");  
    DEBUG_INFO("This is an INFO message test\r\n");
    DEBUG_DEBUG("This is a DEBUG message test\r\n");
    
    printf("Standard printf test - should output to Bluetooth\r\n");
    
    Debug_SetLevel(DEBUG_LEVEL_DEBUG);
    printf("Debug level set to DEBUG - all messages should show\r\n");
    
    Debug_SetLevel(DEBUG_LEVEL_ERROR);
    printf("Debug level set to ERROR - only error messages should show\r\n");
    
    Debug_SetLevel(DEBUG_LEVEL_INFO);
    printf("Debug level restored to INFO\r\n");
}
