/**
  ******************************************************************************
  * @file    cpu.c
  * @author  JFY
  * @version V1.0
  * @date    2020-08-08
  ******************************************************************************
  */ 
#include "cpu.h"

/**
 ******************************************************************
 *	时钟初始化功能函数段
 ******************************************************************
 */
 
//RCC初始化配置
static BOOLEAN RCC_Configuration(void)
{
	/* 开启相关的GPIO外设时钟 */
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE); 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOB, ENABLE); 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOC, ENABLE); 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOD, ENABLE); 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOE, ENABLE); 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOF, ENABLE); 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOG, ENABLE); 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOH, ENABLE); 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOI, ENABLE); 	
	/* 开启CRC时钟 */
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_CRC, ENABLE); 
	/* 使能 USART1 时钟 */
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1, ENABLE);
	/* 使能 USART2 时钟 */
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_USART2, ENABLE);	
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_SYSCFG, ENABLE);
		
	return 1;
}

/**
 ******************************************************************
 *	优先级设置功能函数段
 ******************************************************************
 */

//中断优先级设置
static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* 嵌套向量中断控制器组选择 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* 配置USART1中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* 配置USART2中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}


/**
 ******************************************************************
 *	端口初始化功能函数段
 ******************************************************************
 */

//端口初始化
static void initPort(void)
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	//打印串口
	/* GPIO初始化 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/* 配置U1 Tx引脚为复用功能  */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* 连接 PA9 到 USART1_Tx */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);

	/* 配置U1 Rx引脚为复用功能 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*  连接 PA10 到 USART1_Rx */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	//UART2串口
	/* 配置U2 Tx引脚为复用功能  */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;  
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	/* 连接 PD5 到 USART2_Tx*/
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);

	/* 配置U2 Rx引脚为复用功能 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	/*  连接 PD6 到 USART2_Rx*/
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);


	/* LVDS-TFT配置 */
	//LCD—>R段   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  
	GPIO_Init(GPIOH, &GPIO_InitStructure); 		//LCD_R3
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource9, GPIO_AF_LTDC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOH, &GPIO_InitStructure);		//LCD_R4
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource10, GPIO_AF_LTDC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOH, &GPIO_InitStructure);		//LCD_R5
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource11, GPIO_AF_LTDC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOH, &GPIO_InitStructure);		//LCD_R6
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource12, GPIO_AF_LTDC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOG, &GPIO_InitStructure);		//LCD_R7
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource6, GPIO_AF_LTDC);

	//LCD—>G段
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOH, &GPIO_InitStructure);		//LCD_G2
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource13, GPIO_AF_LTDC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOH, &GPIO_InitStructure);		//LCD_G3
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource14, GPIO_AF_LTDC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOH, &GPIO_InitStructure);		//LCD_G4
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource15, GPIO_AF_LTDC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOI, &GPIO_InitStructure);		//LCD_G5
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource0, GPIO_AF_LTDC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOI, &GPIO_InitStructure);		//LCD_G6
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource1, GPIO_AF_LTDC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOI, &GPIO_InitStructure);		//LCD_G7
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource2, GPIO_AF_LTDC);

	//LCD—>B段
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOG, &GPIO_InitStructure);		//LCD_B3
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource11, GPIO_AF_LTDC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOI, &GPIO_InitStructure);		//LCD_B4
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource4, GPIO_AF_LTDC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOI, &GPIO_InitStructure);		//LCD_B5
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource5, GPIO_AF_LTDC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOI, &GPIO_InitStructure);		//LCD_B6
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource6, GPIO_AF_LTDC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOI, &GPIO_InitStructure);		//LCD_B7
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource7, GPIO_AF_LTDC);

	//控制信号线
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOI, &GPIO_InitStructure);		//LCD_FP
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource9, GPIO_AF_LTDC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOI, &GPIO_InitStructure);		//LCD_LP
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource10, GPIO_AF_LTDC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOG, &GPIO_InitStructure);		//LCD_DCLK
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource7, GPIO_AF_LTDC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOF, &GPIO_InitStructure);		//LCD_BE
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource10, GPIO_AF_LTDC);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIO_InitStructure);		//LCD_BL

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStructure);		//LCD_RST 
	GPIO_SetBits(GPIOA, GPIO_Pin_15);
	 
	//系统运行LED
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  
	GPIO_Init(GPIOB, &GPIO_InitStructure); 	 
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  
	GPIO_Init(GPIOB, &GPIO_InitStructure); 	 
	GPIO_SetBits(GPIOB, GPIO_Pin_1);
		
}


/**
 ******************************************************************
 *	SDRAM功能函数段
 ******************************************************************
 */


/**
  * @brief  延迟一段时间
  * @param  延迟的时间长度
  * @retval None
  */
static void SDRAM_delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for(index = (5000 * nCount); index != 0; index--)
  {
  }
}

//向SDRAM发送命令
//bankx:0,向BANK5上面的SDRAM发送指令
//      1,向BANK6上面的SDRAM发送指令
//cmd:指令(0,正常模式/1,时钟配置使能/2,预充电所有存储区/3,自动刷新/4,加载模式寄存器/5,自刷新/6,掉电)
//refresh:自刷新次数(cmd=3时有效)
//regval:模式寄存器的定义
//返回值:0,正常;1,失败.
static u8 SDRAM_Send_Cmd(u8 bankx, u8 cmd, u8 refresh, INT16U regval)
{
	INT32U retry = 0;
	INT32U tempreg = 0; 
	tempreg |= (cmd << 0);					//设置指令
	tempreg |= (1 << (4 - bankx));			//设置发送指令到bank5还是6
	tempreg |= (refresh << 5);				//设置自刷新次数
	tempreg |= (regval << 9);				//设置模式寄存器的值
	FMC_Bank5_6->SDCMR = tempreg;			//配置寄存器
	while((FMC_Bank5_6->SDSR & (1 << 5)))	//等待指令发送完成 
	{
		retry++;
		if(retry > 0X1FFFFF)
		{
			return 1; 
		}
	}
	return 0;	
}


/* Initialize the external memory. */
static void BOARD_InitSDRAM(void)
{
	u32 sdctrlreg=0,sdtimereg=0;
	u16 mregval=0;
	
	RCC->AHB3ENR|=1<<0;     	//使能FMC时钟  
	RCC->AHB1ENR|=0X1F<<2;		//使能PC/PD/PE/PF/PG时钟  
 
 	sdctrlreg|=1<<0;				//9位列地址
	sdctrlreg|=2<<2;				//13位行地址
	sdctrlreg|=1<<4;				//16位数据位宽
	sdctrlreg|=1<<6;				//4个内部存区(4 BANKS)
	sdctrlreg|=2<<7;				//2个CAS延迟
	sdctrlreg|=0<<9;				//允许写访问
	sdctrlreg|=2<<10;				//SDRAM时钟=HCLK/2=192M/2=96M=10.4ns
	sdctrlreg|=1<<12;				//使能突发访问 
	sdctrlreg|=0<<13;				//读通道延迟0个HCLK
 	FMC_Bank5_6->SDCR[0]=sdctrlreg;	//设置FMC BANK5 SDRAM控制寄存器(BANK5和6用于管理SDRAM).

	sdtimereg|=1<<0;				//加载模式寄存器到激活时间的延迟为2个时钟周期
	sdtimereg|=6<<4;				//退出自刷新延迟为7个时钟周期
	sdtimereg|=5<<8;				//自刷新时间为6个时钟周期
	sdtimereg|=5<<12;				//行循环延迟为6个时钟周期
	sdtimereg|=1<<16;				//恢复延迟为2个时钟周期
	sdtimereg|=1<<20;				//行预充电延迟为2个时钟周期
	sdtimereg|=1<<24;				//行到列延迟为2个时钟周期
 	FMC_Bank5_6->SDTR[0]=sdtimereg;	//设置FMC BANK5 SDRAM时序寄存器 

	SDRAM_Send_Cmd(0,1,0,0);		//时钟配置使能
	SDRAM_delay(500);				//至少延迟200us.
	SDRAM_Send_Cmd(0,2,0,0);		//对所有存储区预充电
	SDRAM_Send_Cmd(0,3,8,0);		//设置自刷新次数 
	mregval|=1<<0;					//设置突发长度:1(可以是1/2/4/8)
	mregval|=0<<3;					//设置突发类型:连续(可以是连续/交错)
	mregval|=2<<4;					//设置CAS值:2(可以是2/3)
	mregval|=0<<7;					//设置操作模式:0,标准模式
	mregval|=1<<9;					//设置突发写模式:1,单点访问
	SDRAM_Send_Cmd(0,4,0,mregval);	//设置SDRAM的模式寄存器
	
	//刷新频率计数器(以SDCLK频率计数),计算方法:
	//COUNT=SDRAM刷新周期/行数-20=SDRAM刷新周期(us)*SDCLK频率(Mhz)/行数
	//我们使用的SDRAM刷新周期为64ms,SDCLK=192/2=96Mhz,行数为8192(2^13).
	//所以,COUNT=64*1000*96/8192-20=730 
	FMC_Bank5_6->SDRTR=730<<1;		//设置刷新频率计数器
	
}

static void initSDRAM(void)
{	
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*-- SDRAM 端口配置 -----------------------------------------------------*/

	/* 通用 GPIO 配置 */
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;       //配置为复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽输出
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;   


	/*A行列地址信号线 针对引脚配置*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource0, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource1, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource2, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource3, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource4, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource5, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource12, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource13, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource14, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource15, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource0, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource1, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource2, GPIO_AF_FMC);


	/*BA地址信号线*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource4, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource5, GPIO_AF_FMC);


	/*DQ数据信号线 针对引脚配置*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; 
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FMC);

	/*控制信号线*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);	//LnSDCS0
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource2 , GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);	//nWE
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource0 , GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);	//LnSDRAS
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource11 , GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; 
	GPIO_Init(GPIOG, &GPIO_InitStructure); //LnSDCAS
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource15 , GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
	GPIO_Init(GPIOG, &GPIO_InitStructure); //LSDCLK0
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource8 , GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);	//LSDCKE0
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource3 , GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_Init(GPIOE, &GPIO_InitStructure);	//LDQM1
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource1 , GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_Init(GPIOE, &GPIO_InitStructure);	//LDQM0
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource0 , GPIO_AF_FMC);
	
	BOARD_InitSDRAM();
	
}


/**
 ******************************************************************
 *	LCD功能函数段
 ******************************************************************
 */

INT16U ltdc_lcd_framebuf[1024][600] __attribute__((at(LCD_FRAME_BUF_ADDR)));	//定义最大屏分辨率时,LCD所需的帧缓存数组大小
INT32U *ltdc_framebuf[2];			//LTDC LCD帧缓存数组指针,必须指向对应大小的内存区域

_ltdc_dev lcdltdc;					//管理LCD LTDC参数
_lcd_dev  lcddev;					//管理LCD重要参数


//LTDC时钟(Fdclk)设置函数
//Fvco=Fin*pllsain; 
//Fdclk=Fvco/pllsair/2*2^pllsaidivr=Fin*pllsain/pllsair/2*2^pllsaidivr;
//返回值:0,成功;1,失败。
static INT8U LTDC_Clk_Set(INT32U pllsain, INT32U pllsair, INT32U pllsaidivr)
{ 
	INT16U retry = 0;
	INT8U status = 0;
	INT32U tempreg = 0;
	RCC->CR &= ~(1<<28);	//关闭SAI时钟 
	while(((RCC->CR & (1<<29))) && (retry < 0X1FFF))
	{
		retry++;//等待SAI时钟失锁
	}
	
 	if(retry == 0X1FFF)
	{
		status = 1;			//LTDC时钟关闭失败
	}
	else   
	{ 
		tempreg |= pllsain<<6;
		tempreg |= pllsair<<28;
		RCC->PLLSAICFGR = tempreg;		//设置LTDC的倍频和分频
		RCC->DCKCFGR &= ~(3<<16);		//清除原来的设置.
		RCC->DCKCFGR |= pllsaidivr<<16;	//设置fdclk分频

		RCC->CR |= 1<<28;					//开启SAI时钟 
		while(((RCC->CR & (1<<29)) == 0) && (retry < 0X1FFF))
		{
			retry++;//等待SAI时钟锁定
		}
		if(retry==0X1FFF)
		{
			status = 2;	
		}
 	} 
	return status;
}  


//开关指定层
//layerx:层号;0,第一层;1,第二层;
//sw:1,打开;0,关闭
static void LTDC_Layer_Switch(INT8U layerx, INT8U sw)
{
	if(sw == 1)
	{
		if(layerx == 0)
		{
			LTDC_Layer1->CR |= 1<<0;	//开启层1
		}
		else 
		{
			LTDC_Layer2->CR |= 1<<0;	//开启层2
		}
	}
	else if(sw == 0)  
	{
		if(layerx == 0)
		{
			LTDC_Layer1->CR &= ~(1<<0);	//关闭层1
		}
		else 
		{
			LTDC_Layer2->CR &= ~(1<<0);	//关闭层2
		}
	}
	LTDC->SRCR |= 1<<0;					//重新加载配置
}


//设置LCD显示方向
//dir:0,竖屏；1,横屏
static void LCD_Display_Dir(INT8U dir)
{
	lcddev.dir = dir;	//竖屏/横屏
	lcdltdc.dir = dir; 	//显示方向
	
	if(dir == 0)		//竖屏
	{
		lcdltdc.width = lcdltdc.pheight;
		lcdltdc.height = lcdltdc.pwidth;	
	}
	else if(dir == 1)	//横屏
	{
		lcdltdc.width = lcdltdc.pwidth;
		lcdltdc.height = lcdltdc.pheight;
	}
	
	lcddev.width = lcdltdc.width;
	lcddev.height = lcdltdc.height;
	return;
	
}

//LTDC,基本参数设置.
//layerx:层值,0/1.
//bufaddr:层颜色帧缓存起始地址
//pixformat:颜色格式.0,ARGB8888;1,RGB888;2,RGB565;3,ARGB1555;4,ARGB4444;5,L8;6;AL44;7;AL88
//alpha:层颜色Alpha值,0,全透明;255,不透明
//alpha0:默认颜色Alpha值,0,全透明;255,不透明
//bfac1:混合系数1,4(100),恒定的Alpha;6(101),像素Alpha*恒定Alpha
//bfac2:混合系数2,5(101),恒定的Alpha;7(111),像素Alpha*恒定Alpha
//bkcolor:层默认颜色,32位,低24位有效,RGB888格式
//返回值:无
static void LTDC_Layer_Parameter_Config(INT8U layerx, INT32U bufaddr, INT8U pixformat, INT8U alpha, INT8U alpha0, INT8U bfac1, INT8U bfac2, INT32U bkcolor)
{
	if(layerx==0)
	{
		LTDC_Layer1->CFBAR = bufaddr;						//设置层颜色帧缓存起始地址
		LTDC_Layer1->PFCR = pixformat;						//设置层颜色格式
		LTDC_Layer1->CACR = alpha;							//设置层颜色Alpha值,255分频;设置255,则不透明
		LTDC_Layer1->DCCR = ((INT32U)alpha0<<24) | bkcolor; 	//设置默认颜色Alpha值,以及默认颜色
		LTDC_Layer1->BFCR = ((INT32U)bfac1<<8) | bfac2;		//设置层混合系数
	}else
	{
		LTDC_Layer2->CFBAR = bufaddr;						//设置层颜色帧缓存起始地址
		LTDC_Layer2->PFCR = pixformat;						//设置层颜色格式
		LTDC_Layer2->CACR = alpha;							//设置层颜色Alpha值,255分频;设置255,则不透明
		LTDC_Layer2->DCCR = ((INT32U)alpha0<<24) | bkcolor;	//设置默认颜色Alpha值,以及默认颜色
		LTDC_Layer2->BFCR = ((INT32U)bfac1<<8) | bfac2;		//设置层混合系数
	}
} 

//LTDC,层窗口设置,窗口以LCD面板坐标系为基准
//layerx:层值,0/1.
//sx,sy:起始坐标
//width,height:宽度和高度
static void LTDC_Layer_Window_Config(INT8U layerx, INT16U sx, INT16U sy, INT16U width, INT16U height)
{
	INT32U temp;
	INT8U pixformat = 0; 
	if(layerx == 0)
	{
		temp = (sx + width + ((LTDC->BPCR & 0X0FFF0000)>>16))<<16; 
		LTDC_Layer1->WHPCR = (sx + ((LTDC->BPCR & 0X0FFF0000)>>16) + 1) | temp; //设置行起始和结束位置 
		temp = (sy + height + (LTDC->BPCR & 0X7FF))<<16; 
		LTDC_Layer1->WVPCR = (sy + (LTDC->BPCR & 0X7FF) + 1) | temp;	//设置列起始和结束位置
		pixformat = LTDC_Layer1->PFCR & 0X07;		//得到颜色格式
		if(pixformat == 0) 
		{
			temp = 4;					//ARGB8888,一个点4个字节
		}
		else if(pixformat == 1)
		{
			temp = 3;					//RGB888,一个点3个字节
		}
		else if(pixformat==5||pixformat==6)
		{
			temp = 1;					//L8/AL44,一个点1个字节
		}
		else 
		{
			temp = 2;					//其他格式,一个点2个字节
		}			
		LTDC_Layer1->CFBLR = (width * temp<<16) | (width * temp + 3);	//帧缓冲区长度设置(字节为单位)
		LTDC_Layer1->CFBLNR = height;	//帧缓冲区行数设置	
	}
	else
	{
		temp = (sx + width + ((LTDC->BPCR & 0X0FFF0000)>>16))<<16; 
		LTDC_Layer2->WHPCR = (sx + ((LTDC->BPCR & 0X0FFF0000)>>16) + 1) | temp;//设置行起始和结束位置 
		temp = (sy + height + (LTDC->BPCR & 0X7FF))<<16; 
		LTDC_Layer2->WVPCR = (sy + (LTDC->BPCR & 0X7FF) + 1) | temp;		//设置列起始和结束位置
		pixformat = LTDC_Layer2->PFCR & 0X07;	//得到颜色格式
		if(pixformat == 0)
		{
			temp = 4;							//ARGB8888,一个点4个字节
		}
		else if(pixformat == 1)
		{
			temp = 3;							//RGB888,一个点3个字节
		}
		else if(pixformat==5||pixformat==6)
		{
			temp=1;		//L8/AL44,一个点1个字节
		}
		else 
		{
			temp = 2;	//其他格式,一个点2个字节
		}			
		LTDC_Layer2->CFBLR = (width * temp<<16) | (width * temp + 3);		//帧缓冲区长度设置(字节为单位)
		LTDC_Layer2->CFBLNR = height;				//帧缓冲区行数设置	
	}	 
	LTDC_Layer_Switch(layerx , 1);					//层使能		
}


//RGBLcd初始化函数
static void initLCD(void)
{   
	INT32U tempreg = 0;

	RCC->APB2ENR |= 1<<26;		//开启LTDC时钟
	
	lcdltdc.pwidth = 1024;		//面板宽度,单位:像素
	lcdltdc.pheight = 600;		//面板高度,单位:像素
	lcdltdc.hsw = 20;			//水平同步宽度
	lcdltdc.vsw = 3;			//垂直同步宽度
	lcdltdc.hbp = 140;			//水平后廊
	lcdltdc.vbp = 20;			//垂直后廊
	lcdltdc.hfp = 160;			//水平前廊
	lcdltdc.vfp = 12;			//垂直前廊
	LTDC_Clk_Set(360 , 2 , 1); 	//设置像素时钟  45Mhz 
	
	tempreg = 0<<28;					//像素时钟极性:不反向
	//tempreg=1<<28;					//像素时钟极性:反向
	tempreg |= 0<<29;					//数据使能极性:低电平有效
	tempreg |= 0<<30;					//垂直同步极性:低电平有效
	tempreg |= 0<<31;					//水平同步极性:低电平有效
	LTDC->GCR = tempreg;				//设置全局控制寄存器 
	tempreg = (lcdltdc.vsw - 1)<<0;		//垂直脉宽-1
	tempreg |= (lcdltdc.hsw - 1)<<16;	//水平脉宽-1
	LTDC->SSCR = tempreg;				//设置同步大小配置寄存器

	tempreg = (lcdltdc.vsw + lcdltdc.vbp - 1)<<0;	//累加垂直后沿=垂直脉宽+垂直后沿-1
	tempreg |= (lcdltdc.hsw + lcdltdc.hbp - 1)<<16;	//累加水平后沿=水平脉宽+水平后沿-1
	LTDC->BPCR = tempreg;							//设置后沿配置寄存器
	
	tempreg = (lcdltdc.vsw + lcdltdc.vbp + lcdltdc.pheight - 1)<<0;	//累加有效高度=垂直脉宽+垂直后沿+垂直分辨率-1
	tempreg |= (lcdltdc.hsw + lcdltdc.hbp + lcdltdc.pwidth - 1)<<16;//累加有效宽度=水平脉宽+水平后沿+水平分辨率-1
	LTDC->AWCR = tempreg;				//设置有效宽度配置寄存器

 	tempreg = (lcdltdc.vsw + lcdltdc.vbp + lcdltdc.pheight + lcdltdc.vfp - 1)<<0;	//总高度=垂直脉宽+垂直后沿+垂直分辨率+垂直前廊-1
	tempreg |= (lcdltdc.hsw + lcdltdc.hbp + lcdltdc.pwidth + lcdltdc.hfp - 1)<<16;	//总宽度=水平脉宽+水平后沿+水平分辨率+水平前廊-1
	LTDC->TWCR = tempreg;				//设置总宽度配置寄存器
	
	LTDC->BCCR = LTDC_BACKLAYERCOLOR;	//设置背景层颜色寄存器(RGB888格式)
	LTDC->GCR |= 1<<0;					//开启LTDC
	
#if LCD_PIXFORMAT == LCD_PIXFORMAT_ARGB8888 || LCD_PIXFORMAT == LCD_PIXFORMAT_RGB888 
	ltdc_framebuf[0] = (INT32U*)&ltdc_lcd_framebuf;
	lcdltdc.pixsize = 4;				//每个像素占4个字节
#else 
	ltdc_framebuf[0] = (INT32U*)&ltdc_lcd_framebuf;
	lcdltdc.pixsize = 2;				//每个像素占2个字节
#endif 	
	//层配置
	LTDC_Layer_Parameter_Config(0 , (INT32U)ltdc_framebuf[0] , LCD_PIXFORMAT , 255 , 0 , 6 , 7 , 0X000000);//层参数配置
	LTDC_Layer_Window_Config(0 , 0 , 0 , lcdltdc.pwidth , lcdltdc.pheight);	//层窗口配置,以LCD面板坐标系为基准,不要随便修改!
	
	lcddev.width = lcdltdc.pwidth;	//设置lcddev的宽度参数
	lcddev.height = lcdltdc.pheight;//设置lcddev的宽度参数		
	lcdltdc.activelayer = 0;		//选择第1层
    LCD_Clear(0XFFFFFFFF);			//清屏
	LCD_Display_Dir(1);				//默认为竖屏 0;竖屏 1：横屏
	LCD_Clear(WHITE);
	
}

//LTDC填充矩形,DMA2D填充
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LTDC_Color_Fill(INT16U sx, INT16U sy, INT16U ex, INT16U ey, INT32U color)
{ 
	INT32U psx, psy, pex, pey;	//以LCD面板为基准的坐标系,不随横竖屏变化而变化
	INT32U timeout = 0; 
	INT16U offline;
	INT32U addr; 
	//坐标系转换
	if(lcdltdc.dir)	//横屏
	{
		psx = sx;
		psy = sy;
		pex = ex;
		pey = ey;
	}else			//竖屏
	{
		psx = sy;
		psy = lcdltdc.pheight - ex - 1;
		pex = ey;
		pey = lcdltdc.pheight - sx - 1;
	} 
	offline = lcdltdc.pwidth - (pex - psx + 1);
	addr = ((INT32U)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * psy + psx));
	RCC->AHB1ENR |= 1<<23;				//使能DM2D时钟
	DMA2D->CR &= ~(1<<0);				//先停止DMA2D
	DMA2D->CR = 3<<16;					//寄存器到存储器模式
	DMA2D->OPFCCR = LCD_PIXFORMAT;		//设置颜色格式
	DMA2D->OOR = offline;				//设置行偏移 
	DMA2D->OMAR = addr;					//输出存储器地址
	DMA2D->NLR = (pey - psy + 1) | ((pex - psx + 1)<<16);	//设定行数寄存器
	DMA2D->OCOLR = color;				//设定输出颜色寄存器 
	DMA2D->CR |= 1<<0;					//启动DMA2D
	while((DMA2D->ISR & (1<<1)) == 0)	//等待传输完成
	{
		timeout++;
		if(timeout > 0X1FFFFF)
		{
			break;	//超时退出
		}
	}  
	DMA2D->IFCR |= 1<<1;				//清除传输完成标志 	
}
  


//LCD清屏
//color:颜色值
void LCD_Clear(INT32U color)
{
	LTDC_Color_Fill(0, 0, lcdltdc.width - 1, lcdltdc.height - 1, color);
}

//LCD开启显示
void LCD_DisplayState(BOOLEAN State)
{					   
	if(State == 1)
	{
		LTDC->GCR |= 1<<0;		//打开LTDC
	}
	else 
	{
		LTDC->GCR &= ~(1<<0);	//关闭LTDC
	}
}

//画点函数
//x,y:写入坐标
//color:颜色值
void LTDC_Draw_Point(INT16U x, INT16U y, INT32U color)
{ 
#if LCD_PIXFORMAT == LCD_PIXFORMAT_ARGB8888 || LCD_PIXFORMAT == LCD_PIXFORMAT_RGB888
	if(lcdltdc.dir)	//横屏
	{
        *(INT32U*)((INT32U)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * y + x)) = color;
	}else 			//竖屏
	{
        *(INT32U*)((INT32U)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y)) = color; 
	}
#else
	if(lcdltdc.dir)	//横屏
	{
        *(INT16U*)((INT32U)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * y + x)) = color;
	}else 			//竖屏
	{
        *(INT16U*)((INT32U)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y)) = color; 
	}
#endif
}


//读点函数
//x,y:读取点的坐标
//返回值:颜色值
INT32U LTDC_Read_Point(INT16U x, INT16U y)
{ 
#if LCD_PIXFORMAT == LCD_PIXFORMAT_ARGB8888 || LCD_PIXFORMAT == LCD_PIXFORMAT_RGB888
	if(lcdltdc.dir)	//横屏
	{
		return *(INT32U*)((INT32U)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * y + x));
	}else 			//竖屏
	{
		return *(INT32U*)((INT32U)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y)); 
	}
#else
	if(lcdltdc.dir)	//横屏
	{
		return *(INT16U*)((INT32U)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * y + x));
	}else 			//竖屏
	{
		return *(INT16U*)((INT32U)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y)); 
	}
#endif 
}


/**
 ******************************************************************
 *	串口功能函数段
 ******************************************************************
 */

//uart1初始化(打印函数)
static void initPrintUart(INT32U Bps)
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = Bps;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure); 
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
}


///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到串口 */
	USART_SendData(USART1, (uint8_t) ch);

	/* 等待发送完毕 */
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		

	return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
	/* 等待串口输入数据 */
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

	return (int)USART_ReceiveData(USART1);
}


/**
 ******************************************************************
 *	系统灯功能函数段
 ******************************************************************
 */

static BOOLEAN p_SystemRunLedState = 0;
//系统灯运行服务函数
void systemRunLedTimer_ISR(void)	
{
	static INT16U s_State;
	
	if(p_SystemRunLedState == 1)
	{
		if(s_State == 0)
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_0);
			s_State = 1;
		}
		else
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_0);
			s_State = 0;
		}
	}
}



//State  1亮  0 灭
void setSystemLedZt(BOOLEAN State)	
{
	p_SystemRunLedState = State;
}



//State  1亮  0 灭
void setBlLedZt(BOOLEAN State)	
{
	if(State == 1)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_5);
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	}
}

/**
 ******************************************************************
 *	硬件初始化功能函数段
 ******************************************************************
 */

//硬件初始化
void hardwareInit(void)
{
	RCC_Configuration();
	NVIC_Configuration();
	initPort();
	initPrintUart(115200);
	initSDRAM();
	initLCD();
	setBlLedZt(1);
}
/* ------------------------------------------end of file---------------------------------------- */
