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
//emwin-task
#include "textdisplay.h"


/*
 *************************************************************************
 *                             ������
 *************************************************************************
 */

/* App������ */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* Main������ */
static TaskHandle_t Main_Task_Handle = NULL;
/* Tick������ */
static TaskHandle_t Tick_Task_Handle = NULL;
/* GUI������ */
static TaskHandle_t GUI_Task_Handle = NULL;
/* Touch������ */
static TaskHandle_t Touch_Task_Handle = NULL;

/*
 *************************************************************************
 *                             ��������
 *************************************************************************
 */
static void AppTaskCreate(void);/* ���ڴ������� */

static void Main_Task(void* p_arg);		/* Main_Task����ʵ�� */
static void Tick_Task(void* p_arg);		/* Main_Task����ʵ�� */
static void GUI_Task(void* p_arg); 		/* GUI_Task����ʵ��  */
static void Touch_Task(void* p_arg); 	/* GUI_Task����ʵ��  */


/*
 *************************************************************************
 *                              ������
 *************************************************************************
 */

int main(void)
{	
	BaseType_t xReturn = pdPASS;	// ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS 
	
	hardwareInit();

	taskENTER_CRITICAL();   //�����ٽ���
	printf("\r\n ********** emwin DEMO *********** \r\n");
	taskEXIT_CRITICAL();	//�˳��ٽ���
  
   /* ����AppTaskCreate���� */
	xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  			/* ������ں��� */
						 (const char*    )"AppTaskCreate",			/* �������� */
						 (uint16_t       )512,  					/* ����ջ��С */
						 (void*          )NULL,						/* ������ں������� */
						 (UBaseType_t    )1, 						/* ��������ȼ� */
						 (TaskHandle_t*  )&AppTaskCreate_Handle);	/* ������ƿ�ָ�� */ 
	/* ����������� */           
	if(pdPASS == xReturn)
		vTaskStartScheduler();   /* �������񣬿������� */
	else
		return -1;  
	while ( 1 )
	{

	}
}


/***********************************************************************
  * @ ������  �� AppTaskCreate
  * @ ����˵���� ���е����񴴽����������������������
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */

	taskENTER_CRITICAL();           //�����ٽ���

	xReturn = xTaskCreate((TaskFunction_t)Main_Task,				/* ������ں��� */
						 (const char*    )"Main_Task",				/* �������� */
						 (uint16_t       )128,       				/* ����ջ��С */
						 (void*          )NULL,      				/* ������ں������� */
						 (UBaseType_t    )4,         				/* ��������ȼ� */
						 (TaskHandle_t   )&Main_Task_Handle);		/* ������ƿ�ָ�� */
	if(pdPASS == xReturn)	
		printf(" creat Main_Task success��\r\n");	
	
	xReturn = xTaskCreate((TaskFunction_t)Tick_Task,				/* ������ں��� */
					   (const char*      )"Tick_Task",				/* �������� */
					   (uint16_t         )128,      				/* ����ջ��С */
					   (void*            )NULL,      				/* ������ں������� */
					   (UBaseType_t      )3,         				/* ��������ȼ� */
					   (TaskHandle_t     )&Tick_Task_Handle);		/* ������ƿ�ָ�� */
	if(pdPASS == xReturn)
		printf(" creat Tick_Task success��\r\n");
	
	xReturn = xTaskCreate((TaskFunction_t)GUI_Task,					/* ������ں��� */
					   (const char*      )"GUI_Task",				/* �������� */
					   (uint16_t         )2048,      				/* ����ջ��С */
					   (void*            )NULL,      				/* ������ں������� */
					   (UBaseType_t      )2,         				/* ��������ȼ� */
					   (TaskHandle_t     )&GUI_Task_Handle);		/* ������ƿ�ָ�� */
	if(pdPASS == xReturn)
		printf(" creat GUI_Task  success��\r\n");
	
	xReturn = xTaskCreate((TaskFunction_t)Touch_Task,				/* ������ں��� */
					   (const char*      )"Touch_Task",				/* �������� */
					   (uint16_t         )2048,      				/* ����ջ��С */
					   (void*            )NULL,      				/* ������ں������� */
					   (UBaseType_t      )1,         				/* ��������ȼ� */
					   (TaskHandle_t     )&Touch_Task_Handle);		/* ������ƿ�ָ�� */
	if(pdPASS == xReturn)
		printf(" creat Touch_Task success��\r\n");

	vTaskDelete(AppTaskCreate_Handle);//ɾ��AppTaskCreate����

	taskEXIT_CRITICAL();//�˳��ٽ���
}

/**
  * @brief 	Main��������
  * @note 	��
  * @param 	��
  * @retval ��
  */
u8 pBuffer[20];
static void Main_Task(void* p_arg)
{
	while(1)
	{
		vTaskDelay(1000);
		setSystemLedZt(1);
	}
}

/**
  * @brief 	Tick��������
  * @note 	��
  * @param 	��
  * @retval ��
  */
static void Tick_Task(void* p_arg)
{
	while(1)
	{
		vTaskDelay(100);
		systemRunLedTimer_ISR();	
	}
}


/**
  * @brief 	GUI��������
  * @note 	��
  * @param 	��
  * @retval ��
  */
static void GUI_Task(void* p_arg)
{
	WM_SetCreateFlags(WM_CF_MEMDEV);
	GUI_Init();
	WM_MULTIBUF_Enable(1);
	WM_MOTION_Enable(1);
	GUI_CURSOR_Show();
	GUIDEMO_Main();
	while(1)
	{
		vTaskDelay(100);
	}
}


/**
  * @brief 	TOUCH��������
  * @note 	��
  * @param 	��
  * @retval ��
  */
static void Touch_Task(void *p_arg) 
{
	while(1)
	{
		GUI_TOUCH_Exec();
		vTaskDelay(1);
	}
	
}



 


/* ------------------------------------------end of file---------------------------------------- */

