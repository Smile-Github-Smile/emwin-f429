/**
  ******************************************************************************
  * @file    bsp_exti.c
  * @author  jfy
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   I/O线中断应用bsp
  ******************************************************************************
  */
  
#include "bsp_exti.h"

 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* 配置中断源：按键WKUP */
	NVIC_InitStructure.NVIC_IRQChannel = WKUP_INT_EXTI_IRQ;
	/* 配置抢占优先级：10 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
	/* 配置子优先级：0 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	/* 使能中断通道 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
  
}

 /**
  * @brief  配置 PA0 为线中断口，并设置中断优先级
  * @param  无
  * @retval 无
  */
void EXTI_Key_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* 开启按键GPIO口的时钟 */
	RCC_AHB1PeriphClockCmd(WKUP_INT_GPIO_CLK, ENABLE);

	/* 使能 SYSCFG 时钟 ，使用GPIO外部中断时必须使能SYSCFG时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* 配置 NVIC */
	NVIC_Configuration();

	/* 选择按键WKUP的引脚 */ 
	GPIO_InitStructure.GPIO_Pin = WKUP_INT_GPIO_PIN;
	/* 设置引脚为输入模式 */ 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	    		
	/* 设置引脚下拉 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	/* 使用上面的结构体初始化按键 */
	GPIO_Init(WKUP_INT_GPIO_PORT, &GPIO_InitStructure); 

	/* 连接 EXTI 中断源到WKUP引脚 */
	SYSCFG_EXTILineConfig(WKUP_INT_EXTI_PORTSOURCE, WKUP_INT_EXTI_PINSOURCE);

	/* 选择 EXTI 中断源 */
	EXTI_InitStructure.EXTI_Line = WKUP_INT_EXTI_LINE;
	/* 中断模式 */
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 下降沿触发 */
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	/* 使能中断/事件线 */
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

}
/*********************************************END OF FILE**********************/
