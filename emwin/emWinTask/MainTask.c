/**
  ******************************************************************************
  * @file    MainTask.c
  * @author  JFY
  * @version V1.0
  * @date    2020-08-08
  ******************************************************************************
  */ 

#include "MainTask.h"


/*******************************************************************************
 * ����
 ******************************************************************************/
/**
  * @brief 	GUI������
  * @note 	��
  * @param 	��
  * @retval ��
  */
void MainTask(void)
{		
	GUI_SetBkColor(GUI_WHITE);
	GUI_Clear();

	GUI_CURSOR_Show();  //��ʾ����ͷ

	GUI_SetColor(GUI_RED);
	GUI_SetFont(GUI_FONT_24B_1);

	GUI_DispString("Hello World! \r\nFREE RTOS\r\n");
	GUI_DispString("YangXingBo!  \r\nFREE RTOS\r\n");
	GUI_DispString("XiaQiu!      \r\nFREE RTOS\r\n");
	GUI_DispString("JiangFaYuan! \r\nFREE RTOS\r\n");

	while(1)
	{
		GUI_Delay(1000);
	}
}



