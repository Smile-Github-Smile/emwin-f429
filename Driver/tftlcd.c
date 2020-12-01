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

INT16U ltdc_lcd_framebuf[1280][800] __attribute__((at(LCD_FRAME_BUF_ADDR)));	//定义最大屏分辨率时,LCD所需的帧缓存数组大小
INT32U *ltdc_framebuf[2];	//LTDC LCD帧缓存数组指针,必须指向对应大小的内存区域


_ltdc_dev lcdltdc;	//管理LCD LTDC的重要参数
_lcd_dev lcddev;	//管理LCD重要参数


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
*	                                 下面的函数被emWin所调用
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*	函 数 名: LCD_ConfigLTDC
*	功能说明: 配置LTDC
*	形    参: 无
*	返 回 值: 无
*   笔    记:
*       LCD_TFT 同步时序配置（整理自官方做的一个截图，言简意赅）：
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
*     每个LCD设备都有自己的同步时序值：
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
*     LCD_TFT 窗口水平和垂直的起始以及结束位置 :
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

	/* 使能LTDC */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);

	/* 使能DMA2D */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE);

	/* 配置信号极性 */	
	LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;	/* HSYNC 低电平有效 */
	LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;	/* VSYNC 低电平有效 */
	LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;	/* DE 低电平有效 */
	LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;

	/* 背景色 */
	LTDC_InitStruct.LTDC_BackgroundRedValue = 0xff;
	LTDC_InitStruct.LTDC_BackgroundGreenValue = 0;
	LTDC_InitStruct.LTDC_BackgroundBlueValue = 0;
	
	/* 
	   LTDC时钟配置说明：
	     函数RCC_PLLSAIConfig的第一个参数是PLLSAI_N，第三个参数数PLLSAI_R。
	     函数RCC_LTDCCLKDivConfig的参数是RCC_PLLSAIDivR。
	   
	   下面举一个例子：PLLSAI_N = 400， PLLSAI_R = 4  RCC_PLLSAIDivR = 2:
	     首先，输入时钟 PLLSAI_VCO Input = HSE_VALUE / PLL_M = 8M / 8 = 1MHz 
	       输出时钟 PLLSAI_VCO Output  = PLLSAI_VCO Input * PLLSAI_N = 1 * 400 = 400 1MHz 
	       PLLLCDCLK = PLLSAI_VCO Output / PLLSAI_R = 400 / 4 = 100 1MHz 
	     最好，LTDC 时钟 = PLLLCDCLK / RCC_PLLSAIDivR = 100 / 2 = 50 1MHz 
	 */
				
	LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;	/* HSYNC 低电平有效 */
	LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;	/* VSYNC 低电平有效 */
	LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;	/* DE 低电平有效 */
	LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IIPC;
	
	/* IPS 7寸 1024*600，  像素时钟频率范围 : 57 -- 65 --- 70.5MHz 

		PLLSAI_VCO Input   = HSE_VALUE / PLL_M = 8M / 4 = 2 Mhz
		PLLSAI_VCO Output  = PLLSAI_VCO Input * PLLSAI_N =   2 * 429 = 858 Mhz
		PLLLCDCLK = PLLSAI_VCO Output / PLLSAI_R = 858 / 4 = 214.5 Mhz
		LTDC clock frequency = PLLLCDCLK / RCC_PLLSAIDivR = 214.5 / 4 = 53.625 Mhz 	

		(429, 2, 4); RCC_PLLSAIDivR_Div4 实测像素时钟 = 53.7M
	*/
	RCC_PLLSAIConfig(429, 2, 6);
	RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div4);

	lcdltdc.width = 1024;
	lcdltdc.height = 600;

	lcdltdc.hsw = 2;	/* =10时，显示错位，20时部分屏可以的,80时全部OK */
	lcdltdc.hbp = 157;
	lcdltdc.hfp = 160;

	lcdltdc.vsw = 2;
	lcdltdc.vbp = 20;
	lcdltdc.vfp = 12;			
	
	/* 使能 PLLSAI */
	RCC_PLLSAICmd(ENABLE);
	/* 等待完成 */
	while(RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET);
	
	/* 配置LTDC的同步时序 */
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


//RGBLcd初始化函数
void initLCD(void)
{   
	INT32U tempreg = 0;

	RCC->APB2ENR |= 1<<26;		//开启LTDC时钟
	
	lcdltdc.pwidth = 1024;		//面板宽度,单位:像素
	lcdltdc.pheight = 600;		//面板高度,单位:像素
	lcdltdc.hsw = 20;			//水平同步宽度
	lcdltdc.vsw = 3;			//垂直同步宽度
	lcdltdc.hbp = 140;			//水平后廊
	lcdltdc.vbp = 20;			//垂直后廊
	lcdltdc.hfp = 160;			//水平前廊
	lcdltdc.vfp = 12;			//垂直前廊
	LTDC_Clk_Set(360 , 2 , 1); 	//设置像素时钟  45Mhz 
	
	tempreg = 0<<28;					//像素时钟极性:不反向
	//tempreg=1<<28;					//像素时钟极性:反向
	tempreg |= 0<<29;					//数据使能极性:低电平有效
	tempreg |= 0<<30;					//垂直同步极性:低电平有效
	tempreg |= 0<<31;					//水平同步极性:低电平有效
	LTDC->GCR = tempreg;				//设置全局控制寄存器 
	tempreg = (lcdltdc.vsw - 1)<<0;		//垂直脉宽-1
	tempreg |= (lcdltdc.hsw - 1)<<16;	//水平脉宽-1
	LTDC->SSCR = tempreg;				//设置同步大小配置寄存器

	tempreg = (lcdltdc.vsw + lcdltdc.vbp - 1)<<0;	//累加垂直后沿=垂直脉宽+垂直后沿-1
	tempreg |= (lcdltdc.hsw + lcdltdc.hbp - 1)<<16;	//累加水平后沿=水平脉宽+水平后沿-1
	LTDC->BPCR = tempreg;							//设置后沿配置寄存器
	
	tempreg = (lcdltdc.vsw + lcdltdc.vbp + lcdltdc.pheight - 1)<<0;	//累加有效高度=垂直脉宽+垂直后沿+垂直分辨率-1
	tempreg |= (lcdltdc.hsw + lcdltdc.hbp + lcdltdc.pwidth - 1)<<16;//累加有效宽度=水平脉宽+水平后沿+水平分辨率-1
	LTDC->AWCR = tempreg;				//设置有效宽度配置寄存器

 	tempreg = (lcdltdc.vsw + lcdltdc.vbp + lcdltdc.pheight + lcdltdc.vfp - 1)<<0;	//总高度=垂直脉宽+垂直后沿+垂直分辨率+垂直前廊-1
	tempreg |= (lcdltdc.hsw + lcdltdc.hbp + lcdltdc.pwidth + lcdltdc.hfp - 1)<<16;	//总宽度=水平脉宽+水平后沿+水平分辨率+水平前廊-1
	LTDC->TWCR = tempreg;				//设置总宽度配置寄存器
	
	LTDC->BCCR = LTDC_BACKLAYERCOLOR;	//设置背景层颜色寄存器(RGB888格式)
	LTDC->GCR |= 1<<0;					//开启LTDC
	
#if LCD_PIXFORMAT == LCD_PIXFORMAT_ARGB8888 || LCD_PIXFORMAT == LCD_PIXFORMAT_RGB888 
	ltdc_framebuf[0] = (INT32U*)&ltdc_lcd_framebuf;
	lcdltdc.pixsize = 4;				//每个像素占4个字节
#else 
	ltdc_framebuf[0] = (INT32U*)&ltdc_lcd_framebuf;
	lcdltdc.pixsize = 2;				//每个像素占2个字节
#endif 	
	//层配置
	LTDC_Layer_Parameter_Config(0 , (INT32U)ltdc_framebuf[0] , LCD_PIXFORMAT , 255 , 0 , 6 , 7 , 0X000000);//层参数配置
	LTDC_Layer_Window_Config(0 , 0 , 0 , lcdltdc.pwidth , lcdltdc.pheight);	//层窗口配置,以LCD面板坐标系为基准,不要随便修改!
	
	lcddev.width = lcdltdc.pwidth;	//设置lcddev的宽度参数
	lcddev.height = lcdltdc.pheight;//设置lcddev的宽度参数	
	lcddev.dir = 1; 				//默认为竖屏 0;竖屏 1：横屏
	lcdltdc.activelayer = 0;		//选择第1层
    LCD_Clear(0XFFFFFFFF);			//清屏
	LCD_Display_Dir(1);				//默认为竖屏 0;竖屏 1：横屏
	LCD_Clear(WHITE);
	
}



//清屏函数
//color:要清屏的填充色
static void LCD_Clear(INT32U color)
{
	LTDC_Clear(color);
} 


//LCD开启显示
static void LCD_DisplayState(BOOLEAN State)
{					   
	if(State == 1)
	{
		LTDC->GCR |= 1<<0;		//打开LTDC
	}
	else 
	{
		LTDC->GCR &= ~(1<<0);	//关闭LTDC
	}
}


//设置LCD显示方向
//dir:0,竖屏；1,横屏
static void LCD_Display_Dir(INT8U dir)
{
	lcdltdc.dir = dir; 	//显示方向
	
	if(dir == 0)		//竖屏
	{
		lcdltdc.width = lcdltdc.pheight;
		lcdltdc.height = lcdltdc.pwidth;	
	}
	else if(dir == 1)	//横屏
	{
		lcdltdc.width = lcdltdc.pwidth;
		lcdltdc.height = lcdltdc.pheight;
	}
	
	lcddev.width = lcdltdc.width;
	lcddev.height = lcdltdc.height;
	return;
	
}



//读取个某点的颜色值	 
//x,y:坐标
//返回值:此点的颜色
INT32U LCD_ReadPoint(INT16U x, INT16U y)
{
	return LTDC_Read_Point(x, y);
}



//画点
void LCD_DrawPoint(INT16U x, INT16U y)
{
	LTDC_Draw_Point(x, y, POINT_COLOR);
}



//快速画点
void LCD_Fast_DrawPoint(INT16U x, INT16U y, INT32U color)
{	   
	LTDC_Draw_Point(x, y, color);
}	



//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Fill(INT16U sx, INT16U sy, INT16U ex, INT16U ey, INT32U color)
{          
	LTDC_Fill(sx, sy, ex, ey, color);
} 



//在指定区域内填充指定颜色块			 
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Color_Fill(INT16U sx, INT16U sy, INT16U ex, INT16U ey, INT16U *color)
{  
	LTDC_Color_Fill(sx, sy, ex, ey, color);
}  



//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(INT16U x1, INT16U y1, INT16U x2, INT16U y2)
{
	INT16U ii; 
	int xerr = 0, yerr = 0, delta_x, delta_y, distance; 
	int incx, incy, uRow, uCol; 
	delta_x = x2 - x1; //计算坐标增量 
	delta_y = y2 - y1; 
	uRow = x1; 
	uCol = y1; 
	if(delta_x > 0) incx = 1; //设置单步方向 
	else if(delta_x == 0) incx = 0;//垂直线 
	else {incx = -1; delta_x = -delta_x;} 
	if(delta_y > 0) incy = 1; 
	else if(delta_y == 0) incy = 0;//水平线 
	else{incy = -1; delta_y = -delta_y;} 
	if( delta_x > delta_y) distance = delta_x; //选取基本增量坐标轴 
	else distance = delta_y; 
	for(ii = 0; ii <= (distance + 1); ii++ )//画线输出 
	{  
		LCD_DrawPoint(uRow, uCol);//画点 
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


//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(INT16U x1, INT16U y1, INT16U x2, INT16U y2)
{
	LCD_DrawLine(x1, y1, x2, y1);
	LCD_DrawLine(x1, y1, x1, y2);
	LCD_DrawLine(x1, y2, x2, y2);
	LCD_DrawLine(x2, y1, x2, y2);
}


//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LCD_Draw_Circle(INT16U x0, INT16U y0, INT8U r)
{
	int a, b;
	int di;
	a = 0;
	b = r;	  
	di = 3 - (r << 1);             //判断下个点位置的标志
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
		//使用Bresenham算法画圆     
		if(di < 0) di += 4 * a + 6;	  
		else
		{
			di += 10 + 4 * (a - b);   
			b--;
		} 						    
	}
} 		



//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24/32
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(INT16U x, INT16U y, INT8U num, INT8U size, INT8U mode)
{  							  
    INT8U temp, t1, t;
	INT16U y0 = y;
	INT8U csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);		//得到字体一个字符对应点阵集所占的字节数	
 	num = num - ' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t = 0; t < csize; t++)
	{   
		if(size == 12)
			temp = ASCII_1206[num][t];  //调用1206字体
		else if(size == 16)
			temp = ASCII_1608[num][t];	//调用1608字体
		else if(size == 24)
			temp = ASCII_2412[num][t];	//调用2412字体
		else if(size == 32)
			temp = ASCII_3216[num][t];	//调用3216字体
		else return;					//没有的字库
		
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
				return;		//超区域了
			}
			
			if((y - y0) == size)
			{
				y = y0;
				x++;
				if(x >= lcddev.width)
				{
					return;	//超区域了
				}
				break;
			}
		}  	 
	}  	    	   	 	  
}   


//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
void LCD_ShowString(INT16U x, INT16U y, INT16U width, INT16U height, INT8U size, INT8S *p)
{         
	INT8U x0 = x;
	width += x;
	height += y;
    while((*p <= '~') && (*p >= ' '))//判断是不是非法字符!
    {       
        if(x >= width)
		{
			x = x0;
			y += size;
		}
        if(y >= height)
		{
			break;//退出
		}
        LCD_ShowChar(x, y, *p, size, 0);
        x += size / 2;
        p++;
    }  
}


//m^n函数
//返回值:m^n次方.
static INT32U LCD_Pow(INT8U m, INT8U n)
{
	INT32U result = 1;	 
	while(n--)
	{
		result *= m;
	}		
	return result;
}	


//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
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

//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
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



//画点函数
//x,y:写入坐标
//color:颜色值
static void LTDC_Draw_Point(INT16U x, INT16U y, INT32U color)
{ 

	if(lcdltdc.dir)	//横屏
	{
        *(INT16U*)((INT32U)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * y + x)) = color;
	}else 			//竖屏
	{
        *(INT16U*)((INT32U)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y)) = color; 
	}

}

//读点函数
//x,y:读取点的坐标
//返回值:颜色值
static INT32U LTDC_Read_Point(INT16U x, INT16U y)
{ 

	if(lcdltdc.dir)	//横屏
	{
		return *(INT16U*)((INT32U)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * y + x));
	}else 			//竖屏
	{
		return *(INT16U*)((INT32U)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y)); 
	}
}


//LTDC填充矩形,DMA2D填充
static void LTDC_Fill(INT16U sx, INT16U sy, INT16U ex, INT16U ey, INT32U color)
{ 
	INT32U psx, psy, pex, pey;	
	INT32U timeout = 0; 
	INT16U offline;
	INT32U addr; 
	//坐标系转换
	if(lcdltdc.dir)	//横屏
	{
		psx = sx;
		psy = sy;
		pex = ex;
		pey = ey;
	}
	else			//竖屏
	{
		psx = sy;
		psy = lcdltdc.pheight - ex - 1;
		pex = ey;
		pey = lcdltdc.pheight - sx - 1;
	} 
	offline = lcdltdc.pwidth - (pex - psx + 1);
	addr = ((INT32U)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * psy + psx));
	RCC->AHB1ENR |= 1<<23;				//使能DM2D时钟
	DMA2D->CR &= ~(1<<0);				//先停止DMA2D
	DMA2D->CR = 3<<16;					//寄存器到存储器模式
	DMA2D->OPFCCR = LCD_PIXFORMAT;		//设置颜色格式
	DMA2D->OOR = offline;				//设置行偏移 
	DMA2D->OMAR = addr;					//输出存储器地址
	DMA2D->NLR = (pey - psy + 1) | ((pex - psx + 1)<<16);	//设定行数寄存器
	DMA2D->OCOLR = color;				//设定输出颜色寄存器 
	DMA2D->CR |= 1<<0;					//启动DMA2D
	while((DMA2D->ISR & (1<<1)) == 0)	//等待传输完成
	{
		timeout++;
		if(timeout > 0X1FFFFF)
		{
			break;	//超时退出
		}
	}  
	DMA2D->IFCR |= 1<<1;				//清除传输完成标志 	
}



//指定区域内填充指定颜色块,DMA2D填充	
static void LTDC_Color_Fill(INT16U sx, INT16U sy, INT16U ex, INT16U ey, INT16U *color)
{
	INT32U psx, psy, pex, pey;	
	INT32U timeout = 0; 
	INT16U offline;
	INT32U addr; 
	//坐标系转换
	if(lcdltdc.dir)	//横屏
	{
		psx = sx;
		psy = sy;
		pex = ex;
		pey = ey;
	}
	else			//竖屏
	{
		psx = sy;
		psy = lcdltdc.pheight - ex - 1;
		pex = ey;
		pey = lcdltdc.pheight - sx - 1;
	}
	
	offline = lcdltdc.pwidth - (pex - psx + 1);
	addr = ((INT32U)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * psy + psx));
	RCC->AHB1ENR |= 1<<23;				//使能DM2D时钟
	DMA2D->CR &= ~(1<<0);				//先停止DMA2D
	DMA2D->CR = 0<<16;					//存储器到存储器模式
	DMA2D->FGPFCCR = LCD_PIXFORMAT;		//设置颜色格式
	DMA2D->FGOR = 0;					//前景层行偏移为0
	DMA2D->OOR = offline;				//设置行偏移 
	DMA2D->FGMAR = (INT32U)color;		//源地址
	DMA2D->OMAR = addr;					//输出存储器地址
	DMA2D->NLR = (pey-psy+1) | ((pex - psx + 1)<<16);	//设定行数寄存器 
	DMA2D->CR |= 1<<0;					//启动DMA2D
	while((DMA2D->ISR & (1<<1)) == 0)	//等待传输完成
	{
		timeout++;
		if(timeout>0X1FFFFF)
		{
			break;	//超时退出
		}
	} 
	DMA2D->IFCR |= 1<<1;				//清除传输完成标志  	
}  


//LCD清屏
static void LTDC_Clear(INT32U color)
{
	LTDC_Fill( 0, 0, lcdltdc.width - 1, lcdltdc.height - 1, color);
}


//开关指定层
//layerx:层号;0,第一层;1,第二层;
//sw:1,打开;0,关闭
static void LTDC_Layer_Switch(INT8U layerx, INT8U sw)
{
	if(sw == 1)
	{
		if(layerx == 0)
		{
			LTDC_Layer1->CR |= 1<<0;	//开启层1
		}
		else 
		{
			LTDC_Layer2->CR |= 1<<0;	//开启层2
		}
	}
	else if(sw == 0)  
	{
		if(layerx == 0)
		{
			LTDC_Layer1->CR &= ~(1<<0);	//关闭层1
		}
		else 
		{
			LTDC_Layer2->CR &= ~(1<<0);	//关闭层2
		}
	}
	LTDC->SRCR |= 1<<0;					//重新加载配置
}




//LTDC时钟(Fdclk)设置函数
//Fvco: VCO频率Fvco=Fin*pllsain
//Fin: 晶振为25M,pllm=25时,Fin=1Mhz
//pllsain: SAI时钟倍频系数N,取值范围:50~432.  
//pllsair: SAI时钟的分频系数R,取值范围:2~7
//pllsaidivr: LCD时钟分频系数,取值范围:0~3,对应分频2^(pllsaidivr+1) 
//Fdclk=1*396/3/2*2^1=396/12=33Mhz
//返回值: 0,成功;1,失败。
static INT8U LTDC_Clk_Set(INT32U pllsain, INT32U pllsair, INT32U pllsaidivr)
{ 
	INT16U retry = 0;
	INT8U status = 0;
	INT32U tempreg = 0;
	RCC->CR &= ~(1<<28);	//关闭SAI时钟 
	while(((RCC->CR & (1<<29))) && (retry < 0X1FFF))
	{
		retry++;//等待SAI时钟失锁
	}
	
 	if(retry == 0X1FFF)
	{
		status = 1;			//LTDC时钟关闭失败
	}
	else   
	{ 
		tempreg |= pllsain<<6;
		tempreg |= pllsair<<28;
		RCC->PLLSAICFGR = tempreg;		//设置LTDC的倍频和分频
		RCC->DCKCFGR &= ~(3<<16);		//清除原来的设置.
		RCC->DCKCFGR |= pllsaidivr<<16;	//设置fdclk分频

		RCC->CR |= 1<<28;					//开启SAI时钟 
		while(((RCC->CR & (1<<29)) == 0) && (retry < 0X1FFF))
		{
			retry++;//等待SAI时钟锁定
		}
		if(retry==0X1FFF)
		{
			status = 2;	
		}
 	} 
	return status;
}  




//LTDC,基本参数设置.
//layerx:层值,0/1.
//bufaddr:层颜色帧缓存起始地址
//pixformat:颜色格式.0,ARGB8888;1,RGB888;2,RGB565;3,ARGB1555;4,ARGB4444;5,L8;6;AL44;7;AL88
//alpha:层颜色Alpha值,0,全透明;255,不透明
//alpha0:默认颜色Alpha值,0,全透明;255,不透明
//bfac1:混合系数1,4(100),恒定的Alpha;6(101),像素Alpha*恒定Alpha
//bfac2:混合系数2,5(101),恒定的Alpha;7(111),像素Alpha*恒定Alpha
//bkcolor:层默认颜色,32位,低24位有效,RGB888格式
//返回值:无
static void LTDC_Layer_Parameter_Config(INT8U layerx, INT32U bufaddr, INT8U pixformat, INT8U alpha, INT8U alpha0, INT8U bfac1, INT8U bfac2, INT32U bkcolor)
{
	if(layerx==0)
	{
		LTDC_Layer1->CFBAR = bufaddr;						//设置层颜色帧缓存起始地址
		LTDC_Layer1->PFCR = pixformat;						//设置层颜色格式
		LTDC_Layer1->CACR = alpha;							//设置层颜色Alpha值,255分频;设置255,则不透明
		LTDC_Layer1->DCCR = ((INT32U)alpha0<<24) | bkcolor; //设置默认颜色Alpha值,以及默认颜色
		LTDC_Layer1->BFCR = ((INT32U)bfac1<<8) | bfac2;		//设置层混合系数
	}else
	{
		LTDC_Layer2->CFBAR = bufaddr;						//设置层颜色帧缓存起始地址
		LTDC_Layer2->PFCR = pixformat;						//设置层颜色格式
		LTDC_Layer2->CACR = alpha;							//设置层颜色Alpha值,255分频;设置255,则不透明
		LTDC_Layer2->DCCR = ((INT32U)alpha0<<24) | bkcolor;	//设置默认颜色Alpha值,以及默认颜色
		LTDC_Layer2->BFCR = ((INT32U)bfac1<<8) | bfac2;		//设置层混合系数
	}
} 



//LTDC,层窗口设置,窗口以LCD面板坐标系为基准
//layerx:层值,0/1.
//sx,sy:起始坐标
//width,height:宽度和高度
static void LTDC_Layer_Window_Config(INT8U layerx, INT16U sx, INT16U sy, INT16U width, INT16U height)
{
	INT32U temp;
	INT8U pixformat = 0; 
	if(layerx == 0)
	{
		temp = (sx + width + ((LTDC->BPCR & 0X0FFF0000)>>16))<<16; 
		LTDC_Layer1->WHPCR = (sx + ((LTDC->BPCR & 0X0FFF0000)>>16) + 1) | temp; //设置行起始和结束位置 
		temp = (sy + height + (LTDC->BPCR & 0X7FF))<<16; 
		LTDC_Layer1->WVPCR = (sy + (LTDC->BPCR & 0X7FF) + 1) | temp;	//设置列起始和结束位置
		pixformat = LTDC_Layer1->PFCR & 0X07;		//得到颜色格式
		if(pixformat == 0) 
		{
			temp = 4;					//ARGB8888,一个点4个字节
		}
		else if(pixformat == 1)
		{
			temp = 3;					//RGB888,一个点3个字节
		}
		else if(pixformat==5||pixformat==6)
		{
			temp = 1;					//L8/AL44,一个点1个字节
		}
		else 
		{
			temp = 2;					//其他格式,一个点2个字节
		}			
		LTDC_Layer1->CFBLR = (width * temp<<16) | (width * temp + 3);	//帧缓冲区长度设置(字节为单位)
		LTDC_Layer1->CFBLNR = height;	//帧缓冲区行数设置	
	}
	else
	{
		temp = (sx + width + ((LTDC->BPCR & 0X0FFF0000)>>16))<<16; 
		LTDC_Layer2->WHPCR = (sx + ((LTDC->BPCR & 0X0FFF0000)>>16) + 1) | temp;//设置行起始和结束位置 
		temp = (sy + height + (LTDC->BPCR & 0X7FF))<<16; 
		LTDC_Layer2->WVPCR = (sy + (LTDC->BPCR & 0X7FF) + 1) | temp;		//设置列起始和结束位置
		pixformat = LTDC_Layer2->PFCR & 0X07;	//得到颜色格式
		if(pixformat == 0)
		{
			temp = 4;							//ARGB8888,一个点4个字节
		}
		else if(pixformat == 1)
		{
			temp = 3;							//RGB888,一个点3个字节
		}
		else if(pixformat==5||pixformat==6)
		{
			temp=1;		//L8/AL44,一个点1个字节
		}
		else 
		{
			temp = 2;	//其他格式,一个点2个字节
		}			
		LTDC_Layer2->CFBLR = (width * temp<<16) | (width * temp + 3);		//帧缓冲区长度设置(字节为单位)
		LTDC_Layer2->CFBLNR = height;				//帧缓冲区行数设置	
	}	 
	LTDC_Layer_Switch(layerx , 1);					//层使能		
}




/* ------------------------------------------end of file---------------------------------------- */

