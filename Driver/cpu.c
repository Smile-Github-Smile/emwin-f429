/**
  ******************************************************************************
  * @file    cpu.c
  * @author  JFY
  * @version V1.0
  * @date    2020-08-08
  ******************************************************************************
  */ 
#include "cpu.h"

extern void initLCD(void);

/**
 ******************************************************************
 *	ʱ�ӳ�ʼ�����ܺ�����
 ******************************************************************
 */
 
//RCC��ʼ������
static BOOLEAN RCC_Configuration(void)
{
	/* ������ص�GPIO����ʱ�� */
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE); 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOB, ENABLE); 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOC, ENABLE); 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOD, ENABLE); 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOE, ENABLE); 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOF, ENABLE); 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOG, ENABLE); 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOH, ENABLE); 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOI, ENABLE); 	
	/* ����CRCʱ�� */
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_CRC, ENABLE); 
	/* ʹ�� USART1 ʱ�� */
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1, ENABLE);
	/* ʹ�� USART2 ʱ�� */
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_USART2, ENABLE);	
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_SYSCFG, ENABLE);
		
	return 1;
}

/**
 ******************************************************************
 *	���ȼ����ù��ܺ�����
 ******************************************************************
 */

//�ж����ȼ�����
static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Ƕ�������жϿ�������ѡ�� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* ����USART1�ж� */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* ����USART2�ж� */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}


/**
 ******************************************************************
 *	�˿ڳ�ʼ�����ܺ�����
 ******************************************************************
 */

//�˿ڳ�ʼ��
static void initPort(void)
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	//��ӡ����
	/* GPIO��ʼ�� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/* ����U1 Tx����Ϊ���ù���  */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* ���� PA9 �� USART1_Tx */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);

	/* ����U1 Rx����Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*  ���� PA10 �� USART1_Rx */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	//UART2����
	/* ����U2 Tx����Ϊ���ù���  */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;  
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	/* ���� PD5 �� USART2_Tx*/
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);

	/* ����U2 Rx����Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	/*  ���� PD6 �� USART2_Rx*/
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);


	/* LVDS-TFT���� */
	//LCD��>R��   
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

	//LCD��>G��
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

	//LCD��>B��
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

	//�����ź���
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
	 
	//ϵͳ����LED
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
 *	SDRAM���ܺ�����
 ******************************************************************
 */


/**
  * @brief  �ӳ�һ��ʱ��
  * @param  �ӳٵ�ʱ�䳤��
  * @retval None
  */
static void SDRAM_delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for(index = (5000 * nCount); index != 0; index--)
  {
  }
}

//��SDRAM��������
//bankx:0,��BANK5�����SDRAM����ָ��
//      1,��BANK6�����SDRAM����ָ��
//cmd:ָ��(0,����ģʽ/1,ʱ������ʹ��/2,Ԥ������д洢��/3,�Զ�ˢ��/4,����ģʽ�Ĵ���/5,��ˢ��/6,����)
//refresh:��ˢ�´���(cmd=3ʱ��Ч)
//regval:ģʽ�Ĵ����Ķ���
//����ֵ:0,����;1,ʧ��.
static u8 SDRAM_Send_Cmd(u8 bankx, u8 cmd, u8 refresh, INT16U regval)
{
	INT32U retry = 0;
	INT32U tempreg = 0; 
	tempreg |= (cmd << 0);					//����ָ��
	tempreg |= (1 << (4 - bankx));			//���÷���ָ�bank5����6
	tempreg |= (refresh << 5);				//������ˢ�´���
	tempreg |= (regval << 9);				//����ģʽ�Ĵ�����ֵ
	FMC_Bank5_6->SDCMR = tempreg;			//���üĴ���
	while((FMC_Bank5_6->SDSR & (1 << 5)))	//�ȴ�ָ������ 
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
	
	RCC->AHB3ENR|=1<<0;     	//ʹ��FMCʱ��  
	RCC->AHB1ENR|=0X1F<<2;		//ʹ��PC/PD/PE/PF/PGʱ��  
 
 	sdctrlreg|=1<<0;				//9λ�е�ַ
	sdctrlreg|=2<<2;				//13λ�е�ַ
	sdctrlreg|=1<<4;				//16λ����λ��
	sdctrlreg|=1<<6;				//4���ڲ�����(4 BANKS)
	sdctrlreg|=2<<7;				//2��CAS�ӳ�
	sdctrlreg|=0<<9;				//����д����
	sdctrlreg|=2<<10;				//SDRAMʱ��=HCLK/2=192M/2=96M=10.4ns
	sdctrlreg|=1<<12;				//ʹ��ͻ������ 
	sdctrlreg|=0<<13;				//��ͨ���ӳ�0��HCLK
 	FMC_Bank5_6->SDCR[0]=sdctrlreg;	//����FMC BANK5 SDRAM���ƼĴ���(BANK5��6���ڹ���SDRAM).

	sdtimereg|=1<<0;				//����ģʽ�Ĵ���������ʱ����ӳ�Ϊ2��ʱ������
	sdtimereg|=6<<4;				//�˳���ˢ���ӳ�Ϊ7��ʱ������
	sdtimereg|=5<<8;				//��ˢ��ʱ��Ϊ6��ʱ������
	sdtimereg|=5<<12;				//��ѭ���ӳ�Ϊ6��ʱ������
	sdtimereg|=1<<16;				//�ָ��ӳ�Ϊ2��ʱ������
	sdtimereg|=1<<20;				//��Ԥ����ӳ�Ϊ2��ʱ������
	sdtimereg|=1<<24;				//�е����ӳ�Ϊ2��ʱ������
 	FMC_Bank5_6->SDTR[0]=sdtimereg;	//����FMC BANK5 SDRAMʱ��Ĵ��� 

	SDRAM_Send_Cmd(0,1,0,0);		//ʱ������ʹ��
	SDRAM_delay(500);				//�����ӳ�200us.
	SDRAM_Send_Cmd(0,2,0,0);		//�����д洢��Ԥ���
	SDRAM_Send_Cmd(0,3,8,0);		//������ˢ�´��� 
	mregval|=1<<0;					//����ͻ������:1(������1/2/4/8)
	mregval|=0<<3;					//����ͻ������:����(����������/����)
	mregval|=2<<4;					//����CASֵ:2(������2/3)
	mregval|=0<<7;					//���ò���ģʽ:0,��׼ģʽ
	mregval|=1<<9;					//����ͻ��дģʽ:1,�������
	SDRAM_Send_Cmd(0,4,0,mregval);	//����SDRAM��ģʽ�Ĵ���
	
	//ˢ��Ƶ�ʼ�����(��SDCLKƵ�ʼ���),���㷽��:
	//COUNT=SDRAMˢ������/����-20=SDRAMˢ������(us)*SDCLKƵ��(Mhz)/����
	//����ʹ�õ�SDRAMˢ������Ϊ64ms,SDCLK=192/2=96Mhz,����Ϊ8192(2^13).
	//����,COUNT=64*1000*96/8192-20=730 
	FMC_Bank5_6->SDRTR=730<<1;		//����ˢ��Ƶ�ʼ�����
	
}

static void initSDRAM(void)
{	
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*-- SDRAM �˿����� -----------------------------------------------------*/

	/* ͨ�� GPIO ���� */
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;       //����Ϊ���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //�������
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;   


	/*A���е�ַ�ź��� �����������*/
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


	/*BA��ַ�ź���*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource4, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource5, GPIO_AF_FMC);


	/*DQ�����ź��� �����������*/
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

	/*�����ź���*/
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
 *	���ڹ��ܺ�����
 ******************************************************************
 */

//uart1��ʼ��(��ӡ����)
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


///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ����� */
	USART_SendData(USART1, (uint8_t) ch);

	/* �ȴ�������� */
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		

	return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
	/* �ȴ������������� */
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

	return (int)USART_ReceiveData(USART1);
}


/**
 ******************************************************************
 *	ϵͳ�ƹ��ܺ�����
 ******************************************************************
 */

static BOOLEAN p_SystemRunLedState = 0;
//ϵͳ�����з�����
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



//State  1��  0 ��
void setSystemLedZt(BOOLEAN State)	
{
	p_SystemRunLedState = State;
}



//State  1��  0 ��
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
 *	Ӳ����ʼ�����ܺ�����
 ******************************************************************
 */

//Ӳ����ʼ��
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
