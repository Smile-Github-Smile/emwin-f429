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
 *	LCD���ܺ�����
 ******************************************************************
 */

INT16U ltdc_lcd_framebuf[1024][600] __attribute__((at(LCD_FRAME_BUF_ADDR)));	//����������ֱ���ʱ,LCD�����֡���������С
INT32U *ltdc_framebuf[2];			//LTDC LCD֡��������ָ��,����ָ���Ӧ��С���ڴ�����

_ltdc_dev lcdltdc;					//����LCD LTDC����
_lcd_dev  lcddev;					//����LCD��Ҫ����


//LTDCʱ��(Fdclk)���ú���
//Fvco=Fin*pllsain; 
//Fdclk=Fvco/pllsair/2*2^pllsaidivr=Fin*pllsain/pllsair/2*2^pllsaidivr;
//����ֵ:0,�ɹ�;1,ʧ�ܡ�
static INT8U LTDC_Clk_Set(INT32U pllsain, INT32U pllsair, INT32U pllsaidivr)
{ 
	INT16U retry = 0;
	INT8U status = 0;
	INT32U tempreg = 0;
	RCC->CR &= ~(1<<28);	//�ر�SAIʱ�� 
	while(((RCC->CR & (1<<29))) && (retry < 0X1FFF))
	{
		retry++;//�ȴ�SAIʱ��ʧ��
	}
	
 	if(retry == 0X1FFF)
	{
		status = 1;			//LTDCʱ�ӹر�ʧ��
	}
	else   
	{ 
		tempreg |= pllsain<<6;
		tempreg |= pllsair<<28;
		RCC->PLLSAICFGR = tempreg;		//����LTDC�ı�Ƶ�ͷ�Ƶ
		RCC->DCKCFGR &= ~(3<<16);		//���ԭ��������.
		RCC->DCKCFGR |= pllsaidivr<<16;	//����fdclk��Ƶ

		RCC->CR |= 1<<28;					//����SAIʱ�� 
		while(((RCC->CR & (1<<29)) == 0) && (retry < 0X1FFF))
		{
			retry++;//�ȴ�SAIʱ������
		}
		if(retry==0X1FFF)
		{
			status = 2;	
		}
 	} 
	return status;
}  


//����ָ����
//layerx:���;0,��һ��;1,�ڶ���;
//sw:1,��;0,�ر�
static void LTDC_Layer_Switch(INT8U layerx, INT8U sw)
{
	if(sw == 1)
	{
		if(layerx == 0)
		{
			LTDC_Layer1->CR |= 1<<0;	//������1
		}
		else 
		{
			LTDC_Layer2->CR |= 1<<0;	//������2
		}
	}
	else if(sw == 0)  
	{
		if(layerx == 0)
		{
			LTDC_Layer1->CR &= ~(1<<0);	//�رղ�1
		}
		else 
		{
			LTDC_Layer2->CR &= ~(1<<0);	//�رղ�2
		}
	}
	LTDC->SRCR |= 1<<0;					//���¼�������
}


//����LCD��ʾ����
//dir:0,������1,����
static void LCD_Display_Dir(INT8U dir)
{
	lcddev.dir = dir;	//����/����
	lcdltdc.dir = dir; 	//��ʾ����
	
	if(dir == 0)		//����
	{
		lcdltdc.width = lcdltdc.pheight;
		lcdltdc.height = lcdltdc.pwidth;	
	}
	else if(dir == 1)	//����
	{
		lcdltdc.width = lcdltdc.pwidth;
		lcdltdc.height = lcdltdc.pheight;
	}
	
	lcddev.width = lcdltdc.width;
	lcddev.height = lcdltdc.height;
	return;
	
}

//LTDC,������������.
//layerx:��ֵ,0/1.
//bufaddr:����ɫ֡������ʼ��ַ
//pixformat:��ɫ��ʽ.0,ARGB8888;1,RGB888;2,RGB565;3,ARGB1555;4,ARGB4444;5,L8;6;AL44;7;AL88
//alpha:����ɫAlphaֵ,0,ȫ͸��;255,��͸��
//alpha0:Ĭ����ɫAlphaֵ,0,ȫ͸��;255,��͸��
//bfac1:���ϵ��1,4(100),�㶨��Alpha;6(101),����Alpha*�㶨Alpha
//bfac2:���ϵ��2,5(101),�㶨��Alpha;7(111),����Alpha*�㶨Alpha
//bkcolor:��Ĭ����ɫ,32λ,��24λ��Ч,RGB888��ʽ
//����ֵ:��
static void LTDC_Layer_Parameter_Config(INT8U layerx, INT32U bufaddr, INT8U pixformat, INT8U alpha, INT8U alpha0, INT8U bfac1, INT8U bfac2, INT32U bkcolor)
{
	if(layerx==0)
	{
		LTDC_Layer1->CFBAR = bufaddr;						//���ò���ɫ֡������ʼ��ַ
		LTDC_Layer1->PFCR = pixformat;						//���ò���ɫ��ʽ
		LTDC_Layer1->CACR = alpha;							//���ò���ɫAlphaֵ,255��Ƶ;����255,��͸��
		LTDC_Layer1->DCCR = ((INT32U)alpha0<<24) | bkcolor; 	//����Ĭ����ɫAlphaֵ,�Լ�Ĭ����ɫ
		LTDC_Layer1->BFCR = ((INT32U)bfac1<<8) | bfac2;		//���ò���ϵ��
	}else
	{
		LTDC_Layer2->CFBAR = bufaddr;						//���ò���ɫ֡������ʼ��ַ
		LTDC_Layer2->PFCR = pixformat;						//���ò���ɫ��ʽ
		LTDC_Layer2->CACR = alpha;							//���ò���ɫAlphaֵ,255��Ƶ;����255,��͸��
		LTDC_Layer2->DCCR = ((INT32U)alpha0<<24) | bkcolor;	//����Ĭ����ɫAlphaֵ,�Լ�Ĭ����ɫ
		LTDC_Layer2->BFCR = ((INT32U)bfac1<<8) | bfac2;		//���ò���ϵ��
	}
} 

//LTDC,�㴰������,������LCD�������ϵΪ��׼
//layerx:��ֵ,0/1.
//sx,sy:��ʼ����
//width,height:��Ⱥ͸߶�
static void LTDC_Layer_Window_Config(INT8U layerx, INT16U sx, INT16U sy, INT16U width, INT16U height)
{
	INT32U temp;
	INT8U pixformat = 0; 
	if(layerx == 0)
	{
		temp = (sx + width + ((LTDC->BPCR & 0X0FFF0000)>>16))<<16; 
		LTDC_Layer1->WHPCR = (sx + ((LTDC->BPCR & 0X0FFF0000)>>16) + 1) | temp; //��������ʼ�ͽ���λ�� 
		temp = (sy + height + (LTDC->BPCR & 0X7FF))<<16; 
		LTDC_Layer1->WVPCR = (sy + (LTDC->BPCR & 0X7FF) + 1) | temp;	//��������ʼ�ͽ���λ��
		pixformat = LTDC_Layer1->PFCR & 0X07;		//�õ���ɫ��ʽ
		if(pixformat == 0) 
		{
			temp = 4;					//ARGB8888,һ����4���ֽ�
		}
		else if(pixformat == 1)
		{
			temp = 3;					//RGB888,һ����3���ֽ�
		}
		else if(pixformat==5||pixformat==6)
		{
			temp = 1;					//L8/AL44,һ����1���ֽ�
		}
		else 
		{
			temp = 2;					//������ʽ,һ����2���ֽ�
		}			
		LTDC_Layer1->CFBLR = (width * temp<<16) | (width * temp + 3);	//֡��������������(�ֽ�Ϊ��λ)
		LTDC_Layer1->CFBLNR = height;	//֡��������������	
	}
	else
	{
		temp = (sx + width + ((LTDC->BPCR & 0X0FFF0000)>>16))<<16; 
		LTDC_Layer2->WHPCR = (sx + ((LTDC->BPCR & 0X0FFF0000)>>16) + 1) | temp;//��������ʼ�ͽ���λ�� 
		temp = (sy + height + (LTDC->BPCR & 0X7FF))<<16; 
		LTDC_Layer2->WVPCR = (sy + (LTDC->BPCR & 0X7FF) + 1) | temp;		//��������ʼ�ͽ���λ��
		pixformat = LTDC_Layer2->PFCR & 0X07;	//�õ���ɫ��ʽ
		if(pixformat == 0)
		{
			temp = 4;							//ARGB8888,һ����4���ֽ�
		}
		else if(pixformat == 1)
		{
			temp = 3;							//RGB888,һ����3���ֽ�
		}
		else if(pixformat==5||pixformat==6)
		{
			temp=1;		//L8/AL44,һ����1���ֽ�
		}
		else 
		{
			temp = 2;	//������ʽ,һ����2���ֽ�
		}			
		LTDC_Layer2->CFBLR = (width * temp<<16) | (width * temp + 3);		//֡��������������(�ֽ�Ϊ��λ)
		LTDC_Layer2->CFBLNR = height;				//֡��������������	
	}	 
	LTDC_Layer_Switch(layerx , 1);					//��ʹ��		
}


//RGBLcd��ʼ������
static void initLCD(void)
{   
	INT32U tempreg = 0;

	RCC->APB2ENR |= 1<<26;		//����LTDCʱ��
	
	lcdltdc.pwidth = 1024;		//�����,��λ:����
	lcdltdc.pheight = 600;		//���߶�,��λ:����
	lcdltdc.hsw = 20;			//ˮƽͬ�����
	lcdltdc.vsw = 3;			//��ֱͬ�����
	lcdltdc.hbp = 140;			//ˮƽ����
	lcdltdc.vbp = 20;			//��ֱ����
	lcdltdc.hfp = 160;			//ˮƽǰ��
	lcdltdc.vfp = 12;			//��ֱǰ��
	LTDC_Clk_Set(360 , 2 , 1); 	//��������ʱ��  45Mhz 
	
	tempreg = 0<<28;					//����ʱ�Ӽ���:������
	//tempreg=1<<28;					//����ʱ�Ӽ���:����
	tempreg |= 0<<29;					//����ʹ�ܼ���:�͵�ƽ��Ч
	tempreg |= 0<<30;					//��ֱͬ������:�͵�ƽ��Ч
	tempreg |= 0<<31;					//ˮƽͬ������:�͵�ƽ��Ч
	LTDC->GCR = tempreg;				//����ȫ�ֿ��ƼĴ��� 
	tempreg = (lcdltdc.vsw - 1)<<0;		//��ֱ����-1
	tempreg |= (lcdltdc.hsw - 1)<<16;	//ˮƽ����-1
	LTDC->SSCR = tempreg;				//����ͬ����С���üĴ���

	tempreg = (lcdltdc.vsw + lcdltdc.vbp - 1)<<0;	//�ۼӴ�ֱ����=��ֱ����+��ֱ����-1
	tempreg |= (lcdltdc.hsw + lcdltdc.hbp - 1)<<16;	//�ۼ�ˮƽ����=ˮƽ����+ˮƽ����-1
	LTDC->BPCR = tempreg;							//���ú������üĴ���
	
	tempreg = (lcdltdc.vsw + lcdltdc.vbp + lcdltdc.pheight - 1)<<0;	//�ۼ���Ч�߶�=��ֱ����+��ֱ����+��ֱ�ֱ���-1
	tempreg |= (lcdltdc.hsw + lcdltdc.hbp + lcdltdc.pwidth - 1)<<16;//�ۼ���Ч���=ˮƽ����+ˮƽ����+ˮƽ�ֱ���-1
	LTDC->AWCR = tempreg;				//������Ч������üĴ���

 	tempreg = (lcdltdc.vsw + lcdltdc.vbp + lcdltdc.pheight + lcdltdc.vfp - 1)<<0;	//�ܸ߶�=��ֱ����+��ֱ����+��ֱ�ֱ���+��ֱǰ��-1
	tempreg |= (lcdltdc.hsw + lcdltdc.hbp + lcdltdc.pwidth + lcdltdc.hfp - 1)<<16;	//�ܿ��=ˮƽ����+ˮƽ����+ˮƽ�ֱ���+ˮƽǰ��-1
	LTDC->TWCR = tempreg;				//�����ܿ�����üĴ���
	
	LTDC->BCCR = LTDC_BACKLAYERCOLOR;	//���ñ�������ɫ�Ĵ���(RGB888��ʽ)
	LTDC->GCR |= 1<<0;					//����LTDC
	
#if LCD_PIXFORMAT == LCD_PIXFORMAT_ARGB8888 || LCD_PIXFORMAT == LCD_PIXFORMAT_RGB888 
	ltdc_framebuf[0] = (INT32U*)&ltdc_lcd_framebuf;
	lcdltdc.pixsize = 4;				//ÿ������ռ4���ֽ�
#else 
	ltdc_framebuf[0] = (INT32U*)&ltdc_lcd_framebuf;
	lcdltdc.pixsize = 2;				//ÿ������ռ2���ֽ�
#endif 	
	//������
	LTDC_Layer_Parameter_Config(0 , (INT32U)ltdc_framebuf[0] , LCD_PIXFORMAT , 255 , 0 , 6 , 7 , 0X000000);//���������
	LTDC_Layer_Window_Config(0 , 0 , 0 , lcdltdc.pwidth , lcdltdc.pheight);	//�㴰������,��LCD�������ϵΪ��׼,��Ҫ����޸�!
	
	lcddev.width = lcdltdc.pwidth;	//����lcddev�Ŀ�Ȳ���
	lcddev.height = lcdltdc.pheight;//����lcddev�Ŀ�Ȳ���		
	lcdltdc.activelayer = 0;		//ѡ���1��
    LCD_Clear(0XFFFFFFFF);			//����
	LCD_Display_Dir(1);				//Ĭ��Ϊ���� 0;���� 1������
	LCD_Clear(WHITE);
	
}

//LTDC������,DMA2D���
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void LTDC_Color_Fill(INT16U sx, INT16U sy, INT16U ex, INT16U ey, INT32U color)
{ 
	INT32U psx, psy, pex, pey;	//��LCD���Ϊ��׼������ϵ,����������仯���仯
	INT32U timeout = 0; 
	INT16U offline;
	INT32U addr; 
	//����ϵת��
	if(lcdltdc.dir)	//����
	{
		psx = sx;
		psy = sy;
		pex = ex;
		pey = ey;
	}else			//����
	{
		psx = sy;
		psy = lcdltdc.pheight - ex - 1;
		pex = ey;
		pey = lcdltdc.pheight - sx - 1;
	} 
	offline = lcdltdc.pwidth - (pex - psx + 1);
	addr = ((INT32U)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * psy + psx));
	RCC->AHB1ENR |= 1<<23;				//ʹ��DM2Dʱ��
	DMA2D->CR &= ~(1<<0);				//��ֹͣDMA2D
	DMA2D->CR = 3<<16;					//�Ĵ������洢��ģʽ
	DMA2D->OPFCCR = LCD_PIXFORMAT;		//������ɫ��ʽ
	DMA2D->OOR = offline;				//������ƫ�� 
	DMA2D->OMAR = addr;					//����洢����ַ
	DMA2D->NLR = (pey - psy + 1) | ((pex - psx + 1)<<16);	//�趨�����Ĵ���
	DMA2D->OCOLR = color;				//�趨�����ɫ�Ĵ��� 
	DMA2D->CR |= 1<<0;					//����DMA2D
	while((DMA2D->ISR & (1<<1)) == 0)	//�ȴ��������
	{
		timeout++;
		if(timeout > 0X1FFFFF)
		{
			break;	//��ʱ�˳�
		}
	}  
	DMA2D->IFCR |= 1<<1;				//���������ɱ�־ 	
}
  


//LCD����
//color:��ɫֵ
void LCD_Clear(INT32U color)
{
	LTDC_Color_Fill(0, 0, lcdltdc.width - 1, lcdltdc.height - 1, color);
}

//LCD������ʾ
void LCD_DisplayState(BOOLEAN State)
{					   
	if(State == 1)
	{
		LTDC->GCR |= 1<<0;		//��LTDC
	}
	else 
	{
		LTDC->GCR &= ~(1<<0);	//�ر�LTDC
	}
}

//���㺯��
//x,y:д������
//color:��ɫֵ
void LTDC_Draw_Point(INT16U x, INT16U y, INT32U color)
{ 
#if LCD_PIXFORMAT == LCD_PIXFORMAT_ARGB8888 || LCD_PIXFORMAT == LCD_PIXFORMAT_RGB888
	if(lcdltdc.dir)	//����
	{
        *(INT32U*)((INT32U)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * y + x)) = color;
	}else 			//����
	{
        *(INT32U*)((INT32U)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y)) = color; 
	}
#else
	if(lcdltdc.dir)	//����
	{
        *(INT16U*)((INT32U)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * y + x)) = color;
	}else 			//����
	{
        *(INT16U*)((INT32U)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y)) = color; 
	}
#endif
}


//���㺯��
//x,y:��ȡ�������
//����ֵ:��ɫֵ
INT32U LTDC_Read_Point(INT16U x, INT16U y)
{ 
#if LCD_PIXFORMAT == LCD_PIXFORMAT_ARGB8888 || LCD_PIXFORMAT == LCD_PIXFORMAT_RGB888
	if(lcdltdc.dir)	//����
	{
		return *(INT32U*)((INT32U)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * y + x));
	}else 			//����
	{
		return *(INT32U*)((INT32U)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y)); 
	}
#else
	if(lcdltdc.dir)	//����
	{
		return *(INT16U*)((INT32U)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * y + x));
	}else 			//����
	{
		return *(INT16U*)((INT32U)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y)); 
	}
#endif 
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
