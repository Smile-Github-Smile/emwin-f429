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

static INT16U testsram[10000] __attribute__((at(0XC1000000)));//����������

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
		printf(" creat GUI_Task  success��\r\n");

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
		setSystemLedZt(1);
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
		systemRunLedTimer_ISR();	
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
//	u32 ts=0; 
//	for(ts = 0; ts < 10000; ts++)
//	{
//		testsram[ts] = ts;//Ԥ���������	 
//  	}
//	for(ts = 0; ts < 10000; ts++)
//	{
//		printf("testsram[%d]:%d\r\n", ts, testsram[ts]);
//	}
	
//	WM_SetCreateFlags(WM_CF_MEMDEV);//����STemWin�洢�豸
//	GUI_Init();					//��ʼ��STemWin
//	WM_MULTIBUF_Enable(1);		//����STemWin�໺�壬RGB�����ܻ��õ�
//	MainTask();

	while(1)
	{
		LCD_DrawLine( 0 , 0 , 200 , 200);
		LCD_DrawRectangle(200 , 200 , 400 , 400);
		LCD_Draw_Circle(500 , 200, 100);
		LCD_Fill(600 , 300 , 800 , 500 , BRRED);
		
		LCD_ShowNum(800 , 200, 123456, 6, 32);
		LCD_ShowxNum(800 , 100, 123456, 6, 32 , 1);
		LCD_ShowString(100 , 500 , 300 , 100 , 32 , "smile hello world");
		vTaskDelay(1000);
	}
}


 


/* ------------------------------------------end of file---------------------------------------- */

