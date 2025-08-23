/*头文件预声明*/
#define __TASK_INIT_GLOBALS
/*头文件预声明结束*/

/*包含头文件*/
#include "Task_init.h"
#include "tim.h"
#include "Ultrasonic.h"  // 添加超声波模块
#include "Debug_Output.h" // 添加调试输出重定向
#include "Data_Sender.h" // 添加数据发送模块
#include "Hardware_Config.h" // 添加硬件配置修复
//#include "MPU6050.h"

/* -------------------------------------------------------------------------- */
static void Servo_Init(void);
extern void Debug_Test(void);  // 调试测试函数声明

/* ---------------------------- Static Variables ---------------------------- */


/* ---------------------------- global functions ---------------------------- */

/**
  * @brief  Create all task
  * @note   
  * @param  parameters: none
  * @retval None
  */
void Task_Init_Start(void *parameters)
{
	taskENTER_CRITICAL();           //进入临界区，禁止任务调度器
	
	Debug_Init();                   //调试输出重定向初始化
	
	// 硬件配置问题修复
	Hardware_Config_Init();         //修复引脚配置冲突
	Hardware_Config_Print_Status(); //打印配置状态
	
	//IMU_Init();                   //陀螺仪初始化
	Ultrasonic_Init();              //超声波模块初始化
	Servo_Init();                   //PWM初始化
  printf("** Peripheral inited successfully. ** \r\n");
	printf("** Debug output redirected to Bluetooth (USART2) ** \r\n");
	
	// 测试调试输出系统
	Debug_Test();
	
	// 测试硬件传感器
	DEBUG_INFO("Testing hardware sensors...\r\n");
	Hardware_Test_MPU6050();
	Hardware_Test_Ultrasonic();
	
	// 测试JSON数据发送功能
	DEBUG_INFO("Testing JSON data transmission...\r\n");
	
	// 发送一次IMU数据测试
	if (DataSender_SendIMU()) {
		DEBUG_INFO("IMU JSON packet sent successfully\r\n");
	} else {
		DEBUG_ERROR("Failed to send IMU JSON packet\r\n");
	}
	
	// 发送一次超声波数据测试  
	if (DataSender_SendUltrasonic()) {
		DEBUG_INFO("Ultrasonic JSON packet sent successfully\r\n");
	} else {
		DEBUG_ERROR("Failed to send Ultrasonic JSON packet\r\n");
	}
	
	// 发送一次组合数据包测试
	if (DataSender_SendCombined()) {
		DEBUG_INFO("Combined JSON packet sent successfully\r\n");
	} else {
		DEBUG_ERROR("Failed to send Combined JSON packet\r\n");
	}
	
	// 检查Data_Sender任务是否在运行
	DEBUG_INFO("Checking FreeRTOS tasks status...\r\n");
	DEBUG_INFO("Task_Init completed successfully, Data_Sender should start soon...\r\n");
	
	HAL_Delay(2000);                //延时，等待外设启动
	vTaskDelete(NULL);              //删除初始化任务
	taskEXIT_CRITICAL();            //退出临界区
}

/* ----------------------- Static Function Definitions ---------------------- */


void Servo_Init(void)
{
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);	
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);	
}
