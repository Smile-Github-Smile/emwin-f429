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
#include "lcd.h"

// FreeRTOS head file, add here.
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "list.h"
#include "portable.h"
#include "FreeRTOSConfig.h"


/*
 *************************************************************************
 *                             ������
 *************************************************************************
 */

/* ���������� */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* Main������ */
static TaskHandle_t Main_Task_Handle = NULL;
/* Main������ */
static TaskHandle_t Tick_Task_Handle = NULL;
/* GUI������ */
static TaskHandle_t GUI_Task_Handle = NULL;

/*
 *************************************************************************
 *                             ��������
 *************************************************************************
 */
static void AppTaskCreate(void);/* ���ڴ������� */

static void Main_Task(void* parameter);/* Main_Task����ʵ�� */
static void Tick_Task(void* parameter);/* Main_Task����ʵ�� */
static void GUI_Task(void* parameter);/* GUI_Task����ʵ�� */


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
		printf(" creat GUI_Task success��\r\n");

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
static void Main_Task(void* parameter)
{

	while(1)
	{
		vTaskDelay(1000);
		SetSystemLedZt(1);

		LTDC_Draw_Point(10, 200, RED);
		
		LCD_DrawLine(400, 0, 500, 400);
		
		LCD_Color_Fill(0 , 0, 100, 100, RED);
		
		LCD_Draw_Circle(200, 200, 100);
		
		LCD_DrawRectangle(300, 300, 350, 350);
		
		LCD_ShowString(400, 80, 240, 24, 32, "LTDC LCD TEST");
		
		
	}
}

/**
  * @brief 	Tick��������
  * @note 	��
  * @param 	��
  * @retval ��
  */
static void Tick_Task(void* parameter)
{
	while(1)
	{
		vTaskDelay(100);
		SystemRunLedTimer_ISR();	
	}
}

/**
  * @brief 	GUI��������
  * @note 	��
  * @param 	��
  * @retval ��
  */
static void GUI_Task(void* parameter)
{
//	GUI_Init();	//��ʼ��STemWin

	while(1)
	{
//		MainTask();
		vTaskDelay(1000);
	}
}


 


/* ------------------------------------------end of file---------------------------------------- */
