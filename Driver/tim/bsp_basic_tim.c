/**
  ******************************************************************************
  * @file    bsp_basic_tim.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ������ʱ����ʱ����
  ******************************************************************************
  */
  
#include "bsp_basic_tim.h"

 /**
  * @brief  ������ʱ�� TIMx,x[6,7]�ж����ȼ�����
  * @param  ��
  * @retval ��
  */
static void TIMx_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	// �����ж���Ϊ4
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);		
	// �����ж���Դ
	NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQn; 	
	// ������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	 
	 // ���������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	// ����TIMx_CLK,x[6,7] 
	RCC_APB1PeriphClockCmd(BASIC_TIM_CLK, ENABLE); 

	/* �ۼ� TIM_Period�������һ�����»����ж�*/		
	//����ʱ����0������499����Ϊ500�Σ�Ϊһ����ʱ����
	TIM_TimeBaseStructure.TIM_Period = 500-1;       

	//��ʱ��ʱ��ԴTIMxCLK = 2 * PCLK1  
	//				PCLK1 = HCLK / 4 
	//				=> TIMxCLK=HCLK/2=SystemCoreClock/2=90MHz
	// �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=500000Hz
	TIM_TimeBaseStructure.TIM_Prescaler = 180-1;	

	// ��ʼ����ʱ��TIMx, x[2,3,4,5]
	TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);


	// �����ʱ�������жϱ�־λ
	TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);

	// ������ʱ�������ж�
	TIM_ITConfig(BASIC_TIM, TIM_IT_Update, ENABLE);

	// ʹ�ܶ�ʱ��
	TIM_Cmd(BASIC_TIM, ENABLE);	
}

/**
  * @brief  ��ʼ��������ʱ����ʱ��1ms����һ���ж�
  * @param  ��
  * @retval ��
  */
void TIMx_Configuration(void)
{
	TIMx_NVIC_Configuration();	

	TIM_Mode_Config();
}

/*********************************************END OF FILE**********************/
