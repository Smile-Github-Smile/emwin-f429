/**
  ******************************************************************************
  * @file    bsp_basic_tim.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   基本定时器定时范例
  ******************************************************************************
  */
  
#include "bsp_basic_tim.h"

 /**
  * @brief  基本定时器 TIMx,x[6,7]中断优先级配置
  * @param  无
  * @retval 无
  */
static void TIMx_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	// 设置中断组为4
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);		
	// 设置中断来源
	NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQn; 	
	// 设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	 
	 // 设置子优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	// 开启TIMx_CLK,x[6,7] 
	RCC_APB1PeriphClockCmd(BASIC_TIM_CLK, ENABLE); 

	/* 累计 TIM_Period个后产生一个更新或者中断*/		
	//当定时器从0计数到499，即为500次，为一个定时周期
	TIM_TimeBaseStructure.TIM_Period = 500-1;       

	//定时器时钟源TIMxCLK = 2 * PCLK1  
	//				PCLK1 = HCLK / 4 
	//				=> TIMxCLK=HCLK/2=SystemCoreClock/2=90MHz
	// 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=500000Hz
	TIM_TimeBaseStructure.TIM_Prescaler = 180-1;	

	// 初始化定时器TIMx, x[2,3,4,5]
	TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);


	// 清除定时器更新中断标志位
	TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);

	// 开启定时器更新中断
	TIM_ITConfig(BASIC_TIM, TIM_IT_Update, ENABLE);

	// 使能定时器
	TIM_Cmd(BASIC_TIM, ENABLE);	
}

/**
  * @brief  初始化基本定时器定时，1ms产生一次中断
  * @param  无
  * @retval 无
  */
void TIMx_Configuration(void)
{
	TIMx_NVIC_Configuration();	

	TIM_Mode_Config();
}

/*********************************************END OF FILE**********************/
