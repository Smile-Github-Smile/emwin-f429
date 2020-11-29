#include "GUI.h"
#include "tftlcd.h"
#include "GUIDRV_Template.h"
#include "GUIDRV_FlexColor.h"



//���ó���,���ڴ�����ʾ��������,������ɫת���������ʾ�ߴ�
void LCD_X_Config(void) 
{
	GUI_DEVICE_CreateAndLink(&GUIDRV_Template_API, GUICC_M565, 0, 0); //������ʾ��������
	LCD_SetSizeEx(0, lcddev.width, lcddev.height);
	LCD_SetVSizeEx(0, lcddev.width, lcddev.height);
	
	GUI_TOUCH_Calibrate(GUI_COORD_X, 0, lcddev.width, 0, lcddev.width-1);   
	GUI_TOUCH_Calibrate(GUI_COORD_Y, 0, lcddev.height, 0, lcddev.height-1);
}

//��ʾ�������Ļص�����
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
