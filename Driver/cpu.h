/**
  ******************************************************************************
  * @file    cpu.h
  * @author  JFY
  * @version V1.0
  * @date    2020-08-08
  ******************************************************************************
  */ 
#ifndef __CPU_H__
#define __CPU_H__

#include "stm32f4xx.h" 
#include "stm32f4xx_rcc.h"
#include "includes.h"

//LCD LTDC��Ҫ������
typedef struct  
{							 
	INT32U pwidth;		//LCD���Ŀ��,�̶�����,������ʾ����ı�
	INT32U pheight;		//LCD���ĸ߶�,�̶�����,������ʾ����ı�
	INT16U hsw;			//ˮƽͬ�����
	INT16U vsw;			//��ֱͬ�����
	INT16U hbp;			//ˮƽ����
	INT16U vbp;			//��ֱ����
	INT16U hfp;			//ˮƽǰ��
	INT16U vfp;			//��ֱǰ�� 
	INT8U activelayer;	//��ǰ����:0/1	
	INT8U dir;			//0,����;1,����;
	INT16U width;		//LCD���
	INT16U height;		//LCD�߶�
	INT32U pixsize;		//ÿ��������ռ�ֽ���
}_ltdc_dev; 	


//LCD��Ҫ������
typedef struct  
{		 	 
	INT16U width;		//LCD ���
	INT16U height;		//LCD �߶�
	INT16U id;			//LCD ID
	INT8U  dir;			//���������������ƣ�0��������1��������	
	INT16U	wramcmd;	//��ʼдgramָ��
	INT16U setxcmd;		//����x����ָ��
	INT16U setycmd;		//����y����ָ�� 
}_lcd_dev; 


/**
  * @brief  LCD Color
  */
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL)
#define LGRAYBLUE        0XA651 //ǳ����ɫ
#define LBBLUE           0X2B12 //ǳ����ɫ


#define LCD_PIXFORMAT_ARGB8888		0X00			//ARGB8888��ʽ
#define LCD_PIXFORMAT_RGB888 		0X01			//RGB888��ʽ
#define LCD_PIXFORMAT_RGB565 		0X02 			//RGB565��ʽ
#define LCD_PIXFORMAT_ARGB1555		0X03			//ARGB1555��ʽ
#define LCD_PIXFORMAT_ARGB4444		0X04			//ARGB4444��ʽ
#define LCD_PIXFORMAT_L8			0X05			//L8��ʽ
#define LCD_PIXFORMAT_AL44			0X06			//AL44��ʽ
#define LCD_PIXFORMAT_AL88 			0X07			//AL88��ʽ


//������ɫ���ظ�ʽ,һ����RGB565
#define LCD_PIXFORMAT				LCD_PIXFORMAT_RGB565	

//����Ĭ�ϱ�������ɫ
#define LTDC_BACKLAYERCOLOR			0X00000000	

//LCD֡�������׵�ַ,���ﶨ����SDRAM����.
#define LCD_FRAME_BUF_ADDR			0XC1000000   


int fputc(int ch, FILE *f);
int fgetc(FILE *f);

void SystemRunLedTimer_ISR(void);
void SetSystemLedZt(BOOLEAN State);
void SetBlLedZt(BOOLEAN State);
void hardwareInit(void);


void LTDC_Color_Fill(INT16U sx, INT16U sy, INT16U ex, INT16U ey, INT32U color);
void LCD_Clear(INT32U Color);
void LCD_DisplayState(BOOLEAN State);
void LTDC_Draw_Point(INT16U x, INT16U y, INT32U color);
INT32U LTDC_Read_Point(INT16U x, INT16U y);

#endif	// __CPU_H__

/* ------------------------------------------end of file---------------------------------------- */
