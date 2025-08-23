/*头文件预声明*/
#define __TASK_INIT_GLOBALS
/*头文件预声明结束*/

/*包含头文件*/
#include "Task_init.h"
#include "tim.h"
//#include "MPU6050.h"

/* -------------------------------------------------------------------------- */
static void Servo_Init(void);

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
	
	//IMU_Init();                   //陀螺仪初始化
	Servo_Init();                   //PWM初始化
  printf("** Peripheral inited successfully. ** \r\n");
	
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
