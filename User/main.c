/**
  ******************************************************************************
  * @file    main.c
  * @author  JFY
  * @version V1.0
  * @date    2020-08-08
  ******************************************************************************
  */ 
// bsp head file, add here.
#include "stm32f4xx.h"
#include "cpu.h"
#include "tftlcd.h"
// FreeRTOS head file, add here.
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "list.h"
#include "portable.h"
#include "FreeRTOSConfig.h"
// emwin head file, add here.
#include "GUI.h"
#include "WM.h"
#include "GUIDEMO.h"

#include "textdisplay.h"


/*
 *************************************************************************
 *                             任务句柄
 *************************************************************************
 */

/* 创建任务句柄 */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* Main任务句柄 */
static TaskHandle_t Main_Task_Handle = NULL;
/* Main任务句柄 */
static TaskHandle_t Tick_Task_Handle = NULL;
/* GUI任务句柄 */
static TaskHandle_t GUI_Task_Handle = NULL;


/*
 *************************************************************************
 *                             函数声明
 *************************************************************************
 */
static void AppTaskCreate(void);/* 用于创建任务 */

static void Main_Task(void* parameter);/* Main_Task任务实现 */
static void Tick_Task(void* parameter);/* Main_Task任务实现 */
static void GUI_Task(void* parameter);/* GUI_Task任务实现 */


/*
 *************************************************************************
 *                              主程序
 *************************************************************************
 */

int main(void)
{	
	BaseType_t xReturn = pdPASS;	// 定义一个创建信息返回值，默认为pdPASS 
	
	hardwareInit();

	taskENTER_CRITICAL();   //进入临界区
	printf("\r\n ********** emwin DEMO *********** \r\n");
	taskEXIT_CRITICAL();	//退出临界区
  
   /* 创建AppTaskCreate任务 */
	xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  			/* 任务入口函数 */
						 (const char*    )"AppTaskCreate",			/* 任务名字 */
						 (uint16_t       )512,  					/* 任务栈大小 */
						 (void*          )NULL,						/* 任务入口函数参数 */
						 (UBaseType_t    )1, 						/* 任务的优先级 */
						 (TaskHandle_t*  )&AppTaskCreate_Handle);	/* 任务控制块指针 */ 
	/* 启动任务调度 */           
	if(pdPASS == xReturn)
		vTaskStartScheduler();   /* 启动任务，开启调度 */
	else
		return -1;  
	while ( 1 )
	{

	}
}


/***********************************************************************
  * @ 函数名  ： AppTaskCreate
  * @ 功能说明： 所有的任务创建函数都放在这个函数里面
  * @ 参数    ： 无  
  * @ 返回值  ： 无
  **********************************************************************/
static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */

	taskENTER_CRITICAL();           //进入临界区

	xReturn = xTaskCreate((TaskFunction_t)Main_Task,				/* 任务入口函数 */
						 (const char*    )"Main_Task",				/* 任务名称 */
						 (uint16_t       )128,       				/* 任务栈大小 */
						 (void*          )NULL,      				/* 任务入口函数参数 */
						 (UBaseType_t    )4,         				/* 任务的优先级 */
						 (TaskHandle_t   )&Main_Task_Handle);		/* 任务控制块指针 */
	if(pdPASS == xReturn)	
		printf(" creat Main_Task success！\r\n");	
	
	xReturn = xTaskCreate((TaskFunction_t)Tick_Task,				/* 任务入口函数 */
					   (const char*      )"Tick_Task",				/* 任务名称 */
					   (uint16_t         )128,      				/* 任务栈大小 */
					   (void*            )NULL,      				/* 任务入口函数参数 */
					   (UBaseType_t      )3,         				/* 任务的优先级 */
					   (TaskHandle_t     )&Tick_Task_Handle);		/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf(" creat Tick_Task success！\r\n");
	
	xReturn = xTaskCreate((TaskFunction_t)GUI_Task,					/* 任务入口函数 */
					   (const char*      )"GUI_Task",				/* 任务名称 */
					   (uint16_t         )2048,      				/* 任务栈大小 */
					   (void*            )NULL,      				/* 任务入口函数参数 */
					   (UBaseType_t      )2,         				/* 任务的优先级 */
					   (TaskHandle_t     )&GUI_Task_Handle);		/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf(" creat GUI_Task  success！\r\n");

	vTaskDelete(AppTaskCreate_Handle);//删除AppTaskCreate任务

	taskEXIT_CRITICAL();//退出临界区
}

/**
  * @brief 	Main任务主体
  * @note 	无
  * @param 	无
  * @retval 无
  */
u8 pBuffer[20];
static void Main_Task(void* parameter)
{
	while(1)
	{
		vTaskDelay(1000);
		setSystemLedZt(1);
	}
}

/**
  * @brief 	Tick任务主体
  * @note 	无
  * @param 	无
  * @retval 无
  */
static void Tick_Task(void* parameter)
{
	while(1)
	{
		vTaskDelay(100);
		systemRunLedTimer_ISR();	
	}
}


/**
  * @brief 	GUI任务主体
  * @note 	无
  * @param 	无
  * @retval 无
  */
static void GUI_Task(void* parameter)
{
	WM_SetCreateFlags(WM_CF_MEMDEV);
	GUI_Init();
	WM_MULTIBUF_Enable(1);
	WM_MOTION_Enable(1);
	
	GUIDEMO_Main();
	while(1)
	{
		vTaskDelay(100);
	}
}


 


/* ------------------------------------------end of file---------------------------------------- */

