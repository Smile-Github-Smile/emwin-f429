#include "GUI.h"
#include "tftlcd.h"
#include "GUIDRV_Template.h"
#include "GUIDRV_FlexColor.h"



//配置程序,用于创建显示驱动器件,设置颜色转换程序和显示尺寸
void LCD_X_Config(void) 
{
	GUI_DEVICE_CreateAndLink(&GUIDRV_Template_API,GUICC_M565,0,0); //创建显示驱动器件
	LCD_SetSizeEx(0,lcddev.width,lcddev.height);
	LCD_SetVSizeEx(0,lcddev.width,lcddev.height);
}

//显示器驱动的回调函数
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
	int r;
	(void) LayerIndex;
	(void) pData;

	switch (Cmd) {
		case LCD_X_INITCONTROLLER: {
			return 0;
		}
			default:
		r = -1;
	}
	return r;
}
