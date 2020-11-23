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


//LCD的画笔颜色和背景色	   
#define POINT_COLOR			0xFF000000		//画笔颜色
#define BACK_COLOR 			0xFFFFFFFF  	//背景色 

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
#define LCD_FRAME_BUF_ADDR			0XC0000000   



//LCD LTDC重要参数集
typedef struct  
{							 
	INT32U 	pwidth;			//LCD宽度
	INT32U 	pheight;		//LCD高度
	INT16U 	hsw;			//水平同步宽度
	INT16U 	vsw;			//垂直同步宽度
	INT16U 	hbp;			//水平后廊
	INT16U 	vbp;			//垂直后廊
	INT16U 	hfp;			//水平前廊
	INT16U 	vfp;			//垂直前廊 
	INT8U 	activelayer;	//当前层编号:0/1	
	INT8U 	dir;			//0,竖屏;1,横屏;
	INT16U 	width;			//LCD宽度
	INT16U 	height;			//LCD高度
	INT32U 	pixsize;		//每个像素所占字节数
}_ltdc_dev; 	


//LCD重要参数集
typedef struct  
{		 	 
	INT16U 	width;		//LCD 宽度
	INT16U 	height;		//LCD 高度
	INT8U  	dir;		//横屏还是竖屏控制：0，竖屏；1，横屏。	
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

