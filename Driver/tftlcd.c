/**
  ******************************************************************************
  * @file    tftlcd.c
  * @author  JFY
  * @version V1.0
  * @date    2020-08-08
  ******************************************************************************
  */ 

#include "tftlcd.h"
#include "font.h"

INT16U ltdc_lcd_framebuf[1280][800] __attribute__((at(LCD_FRAME_BUF_ADDR)));	//����������ֱ���ʱ,LCD�����֡���������С
INT32U *ltdc_framebuf[2];	//LTDC LCD֡��������ָ��,����ָ���Ӧ��С���ڴ�����


_ltdc_dev lcdltdc;	//����LCD LTDC����Ҫ����
_lcd_dev lcddev;	//����LCD��Ҫ����


static void LCD_DisplayState(BOOLEAN State);
static void LTDC_Draw_Point(INT16U x, INT16U y, INT32U color);
static INT32U LTDC_Read_Point(INT16U x, INT16U y);
static void LTDC_Fill(INT16U sx, INT16U sy, INT16U ex, INT16U ey, INT32U color);
static void LTDC_Color_Fill(INT16U sx, INT16U sy, INT16U ex, INT16U ey, INT16U *color);
static void LTDC_Clear(INT32U color);
static void LCD_Clear(INT32U color);
static INT8U LTDC_Clk_Set(INT32U pllsain, INT32U pllsair, INT32U pllsaidivr);
static void LTDC_Layer_Parameter_Config(INT8U layerx, INT32U bufaddr, INT8U pixformat, INT8U alpha, INT8U alpha0, INT8U bfac1, INT8U bfac2, INT32U bkcolor);
static void LTDC_Layer_Window_Config(INT8U layerx, INT16U sx, INT16U sy, INT16U width, INT16U height);
static void LCD_Display_Dir(INT8U dir);



/*
*********************************************************************************************************
*	                                 ����ĺ�����emWin������
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*	�� �� ��: LCD_ConfigLTDC
*	����˵��: ����LTDC
*	��    ��: ��
*	�� �� ֵ: ��
*   ��    ��:
*       LCD_TFT ͬ��ʱ�����ã������Թٷ�����һ����ͼ���Լ����ࣩ��
*       ----------------------------------------------------------------------------
*    
*                                                 Total lcdltdc.width
*                             <--------------------------------------------------->
*                       Hsync width lcdltdc.hbp             Active lcdltdc.width                lcdltdc.hfp
*                             <---><--><--------------------------------------><-->
*                         ____    ____|_______________________________________|____ 
*                             |___|   |                                       |    |
*                                     |                                       |    |
*                         __|         |                                       |    |
*            /|\    /|\  |            |                                       |    |
*             | VSYNC|   |            |                                       |    |
*             |lcdltdc.width\|/  |__          |                                       |    |
*             |     /|\     |         |                                       |    |
*             |  lcdltdc.vbp |      |         |                                       |    |
*             |     \|/_____|_________|_______________________________________|    |
*             |     /|\     |         | / / / / / / / / / / / / / / / / / / / |    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*    Total    |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*    Heigh    |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |Active|      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |Heigh |      |         |/ / / / / / Active Display Area / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |     \|/_____|_________|_______________________________________|    |
*             |     /|\     |                                                      |
*             |  lcdltdc.vfp |      |                                                      |
*            \|/    \|/_____|______________________________________________________|
*            
*     
*     ÿ��LCD�豸�����Լ���ͬ��ʱ��ֵ��
*     Horizontal Synchronization (Hsync) 
*     Horizontal Back Porch (lcdltdc.hbp)       
*     Active lcdltdc.width                      
*     Horizontal Front Porch (lcdltdc.hfp)     
*   
*     Vertical Synchronization (Vsync)  
*     Vertical Back Porch (lcdltdc.vbp)         
*     Active Heigh                       
*     Vertical Front Porch (lcdltdc.vfp)         
*     
*     LCD_TFT ����ˮƽ�ʹ�ֱ����ʼ�Լ�����λ�� :
*     ----------------------------------------------------------------
*   
*     HorizontalStart = (Offset_X + Hsync + lcdltdc.hbp);
*     HorizontalStop  = (Offset_X + Hsync + lcdltdc.hbp + Window_Width - 1); 
*     VarticalStart   = (Offset_Y + Vsync + lcdltdc.vbp);
*     VerticalStop    = (Offset_Y + Vsync + lcdltdc.vbp + Window_Heigh - 1);
*
*********************************************************************************************************
*/

void LCD_ConfigLTDC(void)
{
	
	LTDC_InitTypeDef       LTDC_InitStruct;
	LTDC_Layer_TypeDef     LTDC_Layerx;

	/* ʹ��LTDC */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);

	/* ʹ��DMA2D */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE);

	/* �����źż��� */	
	LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;	/* HSYNC �͵�ƽ��Ч */
	LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;	/* VSYNC �͵�ƽ��Ч */
	LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;	/* DE �͵�ƽ��Ч */
	LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;

	/* ����ɫ */
	LTDC_InitStruct.LTDC_BackgroundRedValue = 0xff;
	LTDC_InitStruct.LTDC_BackgroundGreenValue = 0;
	LTDC_InitStruct.LTDC_BackgroundBlueValue = 0;
	
	/* 
	   LTDCʱ������˵����
	     ����RCC_PLLSAIConfig�ĵ�һ��������PLLSAI_N��������������PLLSAI_R��
	     ����RCC_LTDCCLKDivConfig�Ĳ�����RCC_PLLSAIDivR��
	   
	   �����һ�����ӣ�PLLSAI_N = 400�� PLLSAI_R = 4  RCC_PLLSAIDivR = 2:
	     ���ȣ�����ʱ�� PLLSAI_VCO Input = HSE_VALUE / PLL_M = 8M / 8 = 1MHz 
	       ���ʱ�� PLLSAI_VCO Output  = PLLSAI_VCO Input * PLLSAI_N = 1 * 400 = 400 1MHz 
	       PLLLCDCLK = PLLSAI_VCO Output / PLLSAI_R = 400 / 4 = 100 1MHz 
	     ��ã�LTDC ʱ�� = PLLLCDCLK / RCC_PLLSAIDivR = 100 / 2 = 50 1MHz 
	 */
				
	LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;	/* HSYNC �͵�ƽ��Ч */
	LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;	/* VSYNC �͵�ƽ��Ч */
	LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;	/* DE �͵�ƽ��Ч */
	LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IIPC;
	
	/* IPS 7�� 1024*600��  ����ʱ��Ƶ�ʷ�Χ : 57 -- 65 --- 70.5MHz 

		PLLSAI_VCO Input   = HSE_VALUE / PLL_M = 8M / 4 = 2 Mhz
		PLLSAI_VCO Output  = PLLSAI_VCO Input * PLLSAI_N =   2 * 429 = 858 Mhz
		PLLLCDCLK = PLLSAI_VCO Output / PLLSAI_R = 858 / 4 = 214.5 Mhz
		LTDC clock frequency = PLLLCDCLK / RCC_PLLSAIDivR = 214.5 / 4 = 53.625 Mhz 	

		(429, 2, 4); RCC_PLLSAIDivR_Div4 ʵ������ʱ�� = 53.7M
	*/
	RCC_PLLSAIConfig(429, 2, 6);
	RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div4);

	lcdltdc.width = 1024;
	lcdltdc.height = 600;

	lcdltdc.hsw = 2;	/* =10ʱ����ʾ��λ��20ʱ���������Ե�,80ʱȫ��OK */
	lcdltdc.hbp = 157;
	lcdltdc.hfp = 160;

	lcdltdc.vsw = 2;
	lcdltdc.vbp = 20;
	lcdltdc.vfp = 12;			
	
	/* ʹ�� PLLSAI */
	RCC_PLLSAICmd(ENABLE);
	/* �ȴ���� */
	while(RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET);
	
	/* ����LTDC��ͬ��ʱ�� */
	LTDC_InitStruct.LTDC_HorizontalSync = lcdltdc.hsw;
	LTDC_InitStruct.LTDC_VerticalSync = lcdltdc.vsw;
	LTDC_InitStruct.LTDC_AccumulatedHBP = LTDC_InitStruct.LTDC_HorizontalSync + lcdltdc.hbp;
	LTDC_InitStruct.LTDC_AccumulatedVBP = LTDC_InitStruct.LTDC_VerticalSync + lcdltdc.vbp;
	LTDC_InitStruct.LTDC_AccumulatedActiveW = lcdltdc.width + LTDC_InitStruct.LTDC_AccumulatedHBP;
	LTDC_InitStruct.LTDC_AccumulatedActiveH = lcdltdc.height + LTDC_InitStruct.LTDC_AccumulatedVBP;
	LTDC_InitStruct.LTDC_TotalWidth = LTDC_InitStruct.LTDC_AccumulatedActiveW + lcdltdc.hfp;
	LTDC_InitStruct.LTDC_TotalHeigh = LTDC_InitStruct.LTDC_AccumulatedActiveH + lcdltdc.vfp;

	LTDC_Init(&LTDC_InitStruct);
}


//RGBLcd��ʼ������
void initLCD(void)
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
	lcddev.dir = 1; 				//Ĭ��Ϊ���� 0;���� 1������
	lcdltdc.activelayer = 0;		//ѡ���1��
    LCD_Clear(0XFFFFFFFF);			//����
	LCD_Display_Dir(1);				//Ĭ��Ϊ���� 0;���� 1������
	LCD_Clear(WHITE);
	
}



//��������
//color:Ҫ���������ɫ
static void LCD_Clear(INT32U color)
{
	LTDC_Clear(color);
} 


//LCD������ʾ
static void LCD_DisplayState(BOOLEAN State)
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


//����LCD��ʾ����
//dir:0,������1,����
static void LCD_Display_Dir(INT8U dir)
{
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



//��ȡ��ĳ�����ɫֵ	 
//x,y:����
//����ֵ:�˵����ɫ
INT32U LCD_ReadPoint(INT16U x, INT16U y)
{
	return LTDC_Read_Point(x, y);
}



//����
void LCD_DrawPoint(INT16U x, INT16U y)
{
	LTDC_Draw_Point(x, y, POINT_COLOR);
}



//���ٻ���
void LCD_Fast_DrawPoint(INT16U x, INT16U y, INT32U color)
{	   
	LTDC_Draw_Point(x, y, color);
}	



//��ָ����������䵥����ɫ
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void LCD_Fill(INT16U sx, INT16U sy, INT16U ex, INT16U ey, INT32U color)
{          
	LTDC_Fill(sx, sy, ex, ey, color);
} 



//��ָ�����������ָ����ɫ��			 
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void LCD_Color_Fill(INT16U sx, INT16U sy, INT16U ex, INT16U ey, INT16U *color)
{  
	LTDC_Color_Fill(sx, sy, ex, ey, color);
}  



//����
//x1,y1:�������
//x2,y2:�յ�����  
void LCD_DrawLine(INT16U x1, INT16U y1, INT16U x2, INT16U y2)
{
	INT16U ii; 
	int xerr = 0, yerr = 0, delta_x, delta_y, distance; 
	int incx, incy, uRow, uCol; 
	delta_x = x2 - x1; //������������ 
	delta_y = y2 - y1; 
	uRow = x1; 
	uCol = y1; 
	if(delta_x > 0) incx = 1; //���õ������� 
	else if(delta_x == 0) incx = 0;//��ֱ�� 
	else {incx = -1; delta_x = -delta_x;} 
	if(delta_y > 0) incy = 1; 
	else if(delta_y == 0) incy = 0;//ˮƽ�� 
	else{incy = -1; delta_y = -delta_y;} 
	if( delta_x > delta_y) distance = delta_x; //ѡȡ�������������� 
	else distance = delta_y; 
	for(ii = 0; ii <= (distance + 1); ii++ )//������� 
	{  
		LCD_DrawPoint(uRow, uCol);//���� 
		xerr += delta_x; 
		yerr += delta_y; 
		if(xerr > distance) 
		{ 
			xerr -= distance; 
			uRow += incx; 
		} 
		if(yerr > distance) 
		{ 
			yerr -= distance; 
			uCol += incy; 
		} 
	}  
}


//������	  
//(x1,y1),(x2,y2):���εĶԽ�����
void LCD_DrawRectangle(INT16U x1, INT16U y1, INT16U x2, INT16U y2)
{
	LCD_DrawLine(x1, y1, x2, y1);
	LCD_DrawLine(x1, y1, x1, y2);
	LCD_DrawLine(x1, y2, x2, y2);
	LCD_DrawLine(x2, y1, x2, y2);
}


//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void LCD_Draw_Circle(INT16U x0, INT16U y0, INT8U r)
{
	int a, b;
	int di;
	a = 0;
	b = r;	  
	di = 3 - (r << 1);             //�ж��¸���λ�õı�־
	while(a <= b)
	{
		LCD_DrawPoint(x0 + a, y0 - b);             //5
 		LCD_DrawPoint(x0 + b, y0 - a);             //0           
		LCD_DrawPoint(x0 + b, y0 + a);             //4               
		LCD_DrawPoint(x0 + a, y0 + b);             //6 
		LCD_DrawPoint(x0 - a, y0 + b);             //1       
 		LCD_DrawPoint(x0 - b, y0 + a);             
		LCD_DrawPoint(x0 - a, y0 - b);             //2             
  		LCD_DrawPoint(x0 - b, y0 - a);             //7     	         
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di < 0) di += 4 * a + 6;	  
		else
		{
			di += 10 + 4 * (a - b);   
			b--;
		} 						    
	}
} 		



//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16/24/32
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(INT16U x, INT16U y, INT8U num, INT8U size, INT8U mode)
{  							  
    INT8U temp, t1, t;
	INT16U y0 = y;
	INT8U csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���	
 	num = num - ' ';//�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
	for(t = 0; t < csize; t++)
	{   
		if(size == 12)
			temp = ASCII_1206[num][t];  //����1206����
		else if(size == 16)
			temp = ASCII_1608[num][t];	//����1608����
		else if(size == 24)
			temp = ASCII_2412[num][t];	//����2412����
		else if(size == 32)
			temp = ASCII_3216[num][t];	//����3216����
		else return;					//û�е��ֿ�
		
		for(t1 = 0; t1 < 8; t1++)
		{			    
			if(temp & 0x80)
				LCD_Fast_DrawPoint(x, y, POINT_COLOR);
			else if(mode == 0)
				LCD_Fast_DrawPoint(x, y, BACK_COLOR);
			
			temp <<= 1;
			y++;
			
			if(y >= lcddev.height)
			{
				return;		//��������
			}
			
			if((y - y0) == size)
			{
				y = y0;
				x++;
				if(x >= lcddev.width)
				{
					return;	//��������
				}
				break;
			}
		}  	 
	}  	    	   	 	  
}   


//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void LCD_ShowString(INT16U x, INT16U y, INT16U width, INT16U height, INT8U size, INT8S *p)
{         
	INT8U x0 = x;
	width += x;
	height += y;
    while((*p <= '~') && (*p >= ' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x >= width)
		{
			x = x0;
			y += size;
		}
        if(y >= height)
		{
			break;//�˳�
		}
        LCD_ShowChar(x, y, *p, size, 0);
        x += size / 2;
        p++;
    }  
}


//m^n����
//����ֵ:m^n�η�.
static INT32U LCD_Pow(INT8U m, INT8U n)
{
	INT32U result = 1;	 
	while(n--)
	{
		result *= m;
	}		
	return result;
}	


//��ʾ����,��λΪ0,����ʾ
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ 
//num:��ֵ(0~4294967295);	 
void LCD_ShowNum(INT16U x, INT16U y, INT32U num, INT8U len, INT8U size)
{         	
	INT8U t, temp;
	INT8U enshow = 0;
	
	for(t = 0; t < len; t++)
	{
		temp = (num / LCD_Pow(10 , len - t - 1)) % 10;
		
		if((enshow == 0) && (t < (len - 1)))
		{
			if(temp == 0)
			{
				LCD_ShowChar(x + (size / 2) * t, y, ' ', size, 0);
				continue;
			}
			else
			{				
				enshow = 1; 
			}
		 	 
		}
	 	LCD_ShowChar(x + (size / 2) * t, y, temp + '0', size, 0); 
	}
} 

//��ʾ����,��λΪ0,������ʾ
//x,y:�������
//num:��ֵ(0~999999999);	 
//len:����(��Ҫ��ʾ��λ��)
//size:�����С
//mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
void LCD_ShowxNum(INT16U x, INT16U y, INT32U num, INT8U len, INT8U size, INT8U mode)
{  
	INT8U t, temp;
	INT8U enshow = 0;						   
	for(t = 0; t < len; t++)
	{
		temp = (num / LCD_Pow(10, len - t - 1)) % 10;
		
		if((enshow == 0) && (t < (len - 1)))
		{
			if(temp == 0)
			{
				if(mode & 0X80)
				{
					LCD_ShowChar(x + (size / 2) * t, y, '0', size, mode & 0X01);
				}					
				else 
				{
					LCD_ShowChar(x + (size / 2) * t, y, ' ', size, mode & 0X01); 
				}					
 				continue;
			}
			else 
			{
				enshow = 1; 
			}
		 	 
		}
	 	LCD_ShowChar(x + (size / 2) * t, y, temp + '0', size, mode & 0X01); 
	}
} 



//���㺯��
//x,y:д������
//color:��ɫֵ
static void LTDC_Draw_Point(INT16U x, INT16U y, INT32U color)
{ 

	if(lcdltdc.dir)	//����
	{
        *(INT16U*)((INT32U)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * y + x)) = color;
	}else 			//����
	{
        *(INT16U*)((INT32U)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y)) = color; 
	}

}

//���㺯��
//x,y:��ȡ�������
//����ֵ:��ɫֵ
static INT32U LTDC_Read_Point(INT16U x, INT16U y)
{ 

	if(lcdltdc.dir)	//����
	{
		return *(INT16U*)((INT32U)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * y + x));
	}else 			//����
	{
		return *(INT16U*)((INT32U)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y)); 
	}
}


//LTDC������,DMA2D���
static void LTDC_Fill(INT16U sx, INT16U sy, INT16U ex, INT16U ey, INT32U color)
{ 
	INT32U psx, psy, pex, pey;	
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
	}
	else			//����
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



//ָ�����������ָ����ɫ��,DMA2D���	
static void LTDC_Color_Fill(INT16U sx, INT16U sy, INT16U ex, INT16U ey, INT16U *color)
{
	INT32U psx, psy, pex, pey;	
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
	}
	else			//����
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
	DMA2D->CR = 0<<16;					//�洢�����洢��ģʽ
	DMA2D->FGPFCCR = LCD_PIXFORMAT;		//������ɫ��ʽ
	DMA2D->FGOR = 0;					//ǰ������ƫ��Ϊ0
	DMA2D->OOR = offline;				//������ƫ�� 
	DMA2D->FGMAR = (INT32U)color;		//Դ��ַ
	DMA2D->OMAR = addr;					//����洢����ַ
	DMA2D->NLR = (pey-psy+1) | ((pex - psx + 1)<<16);	//�趨�����Ĵ��� 
	DMA2D->CR |= 1<<0;					//����DMA2D
	while((DMA2D->ISR & (1<<1)) == 0)	//�ȴ��������
	{
		timeout++;
		if(timeout>0X1FFFFF)
		{
			break;	//��ʱ�˳�
		}
	} 
	DMA2D->IFCR |= 1<<1;				//���������ɱ�־  	
}  


//LCD����
static void LTDC_Clear(INT32U color)
{
	LTDC_Fill( 0, 0, lcdltdc.width - 1, lcdltdc.height - 1, color);
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




//LTDCʱ��(Fdclk)���ú���
//Fvco: VCOƵ��Fvco=Fin*pllsain
//Fin: ����Ϊ25M,pllm=25ʱ,Fin=1Mhz
//pllsain: SAIʱ�ӱ�Ƶϵ��N,ȡֵ��Χ:50~432.  
//pllsair: SAIʱ�ӵķ�Ƶϵ��R,ȡֵ��Χ:2~7
//pllsaidivr: LCDʱ�ӷ�Ƶϵ��,ȡֵ��Χ:0~3,��Ӧ��Ƶ2^(pllsaidivr+1) 
//Fdclk=1*396/3/2*2^1=396/12=33Mhz
//����ֵ: 0,�ɹ�;1,ʧ�ܡ�
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
		LTDC_Layer1->DCCR = ((INT32U)alpha0<<24) | bkcolor; //����Ĭ����ɫAlphaֵ,�Լ�Ĭ����ɫ
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




/* ------------------------------------------end of file---------------------------------------- */

