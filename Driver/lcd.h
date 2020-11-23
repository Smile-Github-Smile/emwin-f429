/**
  ******************************************************************************
  * @file    lcd.h
  * @author  JFY
  * @version V1.0
  * @date    2020-08-08
  ******************************************************************************
  */ 

#ifndef __LCD_H
#define __LCD_H

#include "stm32f4xx.h"
#include "includes.h"
#include "cpu.h"


//LCD�Ļ�����ɫ�ͱ���ɫ	   
#define POINT_COLOR			0xFF000000		//������ɫ
#define BACK_COLOR 			0xFFFFFFFF  	//����ɫ 

#define LCD_HEIGHT			600		
#define LCD_WIDTH			1024

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
#define LCD_FRAME_BUF_ADDR			0XC0000000   



//LCD LTDC��Ҫ������
typedef struct  
{							 
	INT32U 	pwidth;			//LCD���
	INT32U 	pheight;		//LCD�߶�
	INT16U 	hsw;			//ˮƽͬ�����
	INT16U 	vsw;			//��ֱͬ�����
	INT16U 	hbp;			//ˮƽ����
	INT16U 	vbp;			//��ֱ����
	INT16U 	hfp;			//ˮƽǰ��
	INT16U 	vfp;			//��ֱǰ�� 
	INT8U 	activelayer;	//��ǰ����:0/1	
	INT8U 	dir;			//0,����;1,����;
	INT16U 	width;			//LCD���
	INT16U 	height;			//LCD�߶�
	INT32U 	pixsize;		//ÿ��������ռ�ֽ���
}_ltdc_dev; 	


//LCD��Ҫ������
typedef struct  
{		 	 
	INT16U 	width;		//LCD ���
	INT16U 	height;		//LCD �߶�
	INT8U  	dir;		//���������������ƣ�0��������1��������	
}_lcd_dev; 


void initLCD(void);
INT32U LCD_ReadPoint(INT16U x, INT16U y);
void LCD_Fast_DrawPoint(INT16U x, INT16U y, INT32U color);
void LCD_DrawLine(INT16U x1, INT16U y1, INT16U x2, INT16U y2);
void LCD_DrawRectangle(INT16U x1, INT16U y1, INT16U x2, INT16U y2);
void LCD_Draw_Circle(INT16U x0, INT16U y0, INT8U r);
void LCD_Color_Fill(INT16U sx, INT16U sy, INT16U ex, INT16U ey, INT16U *color);
void LCD_ShowNum(INT16U x, INT16U y, INT32U num, INT8U len, INT8U size);
void LCD_ShowxNum(INT16U x, INT16U y, INT32U num, INT8U len, INT8U size, INT8U mode);
void LCD_ShowString(INT16U x, INT16U y, INT16U width, INT16U height, INT8U size, INT8U *p);

#endif // __LCD_H

/* ------------------------------------------end of file---------------------------------------- */

