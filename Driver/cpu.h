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

//LCD LTDC重要参数集
typedef struct  
{							 
	INT32U pwidth;		//LCD面板的宽度,固定参数,不随显示方向改变
	INT32U pheight;		//LCD面板的高度,固定参数,不随显示方向改变
	INT16U hsw;			//水平同步宽度
	INT16U vsw;			//垂直同步宽度
	INT16U hbp;			//水平后廊
	INT16U vbp;			//垂直后廊
	INT16U hfp;			//水平前廊
	INT16U vfp;			//垂直前廊 
	INT8U activelayer;	//当前层编号:0/1	
	INT8U dir;			//0,竖屏;1,横屏;
	INT16U width;		//LCD宽度
	INT16U height;		//LCD高度
	INT32U pixsize;		//每个像素所占字节数
}_ltdc_dev; 	


//LCD重要参数集
typedef struct  
{		 	 
	INT16U width;		//LCD 宽度
	INT16U height;		//LCD 高度
	INT16U id;			//LCD ID
	INT8U  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	INT16U	wramcmd;	//开始写gram指令
	INT16U setxcmd;		//设置x坐标指令
	INT16U setycmd;		//设置y坐标指令 
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
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL)
#define LGRAYBLUE        0XA651 //浅灰蓝色
#define LBBLUE           0X2B12 //浅棕蓝色


#define LCD_PIXFORMAT_ARGB8888		0X00			//ARGB8888格式
#define LCD_PIXFORMAT_RGB888 		0X01			//RGB888格式
#define LCD_PIXFORMAT_RGB565 		0X02 			//RGB565格式
#define LCD_PIXFORMAT_ARGB1555		0X03			//ARGB1555格式
#define LCD_PIXFORMAT_ARGB4444		0X04			//ARGB4444格式
#define LCD_PIXFORMAT_L8			0X05			//L8格式
#define LCD_PIXFORMAT_AL44			0X06			//AL44格式
#define LCD_PIXFORMAT_AL88 			0X07			//AL88格式


//定义颜色像素格式,一般用RGB565
#define LCD_PIXFORMAT				LCD_PIXFORMAT_RGB565	

//定义默认背景层颜色
#define LTDC_BACKLAYERCOLOR			0X00000000	

//LCD帧缓冲区首地址,这里定义在SDRAM里面.
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
