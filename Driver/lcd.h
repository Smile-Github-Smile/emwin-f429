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


//LCDµÄ»­±ÊÑÕÉ«ºÍ±³¾°É«	   
#define POINT_COLOR			0xFF000000		//»­±ÊÑÕÉ«
#define BACK_COLOR 			0xFFFFFFFF  	//±³¾°É« 

#define LCD_HEIGHT			600		
#define LCD_WIDTH			1024


void LTDC_Draw_Point(INT16U x, INT16U y, INT32U color);
void LCD_Fast_DrawPoint(INT16U x, INT16U y, INT32U color);
INT32U LCD_ReadPoint(INT16U x, INT16U y);
void LCD_DrawLine(INT16U x1, INT16U y1, INT16U x2, INT16U y2);
void LCD_DrawRectangle(INT16U x1, INT16U y1, INT16U x2, INT16U y2);
void LCD_Draw_Circle(INT16U x0, INT16U y0, INT8U r);
void LCD_Color_Fill(INT16U sx, INT16U sy, INT16U ex, INT16U ey, INT32U color);

void LCD_ShowNum(INT16U x, INT16U y, INT32U num, INT8U len, INT8U size);
void LCD_ShowxNum(INT16U x, INT16U y, INT32U num, INT8U len, INT8U size, INT8U mode);
void LCD_ShowString(INT16U x, INT16U y, INT16U width, INT16U height, INT8U size, INT8U *p);

#endif // __LCD_H

/* ------------------------------------------end of file---------------------------------------- */

