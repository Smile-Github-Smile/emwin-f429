/**
  ******************************************************************************
  * @file    lcd.c
  * @author  JFY
  * @version V1.0
  * @date    2020-08-08
  ******************************************************************************
  */ 

#include "lcd.h"
#include "font.h"



//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
static void LCD_DrawPoint(INT16U x, INT16U y)
{
	LTDC_Draw_Point(x, y, POINT_COLOR);
}

//快速画点
//x,y:坐标
//color:颜色
void LCD_Fast_DrawPoint(INT16U x, INT16U y, INT32U color)
{	   
	LTDC_Draw_Point(x, y, color);
}

//读取个某点的颜色值	 
//x,y:坐标
//返回值:此点的颜色
INT32U LCD_ReadPoint(INT16U x, INT16U y)
{
	return LTDC_Read_Point(x, y);
}

//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Color_Fill(INT16U sx, INT16U sy, INT16U ex, INT16U ey, INT32U color)
{          
	LTDC_Color_Fill(sx, sy, ex, ey, color);
} 


//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(INT16U x1, INT16U y1, INT16U x2, INT16U y2)
{
	INT16U t; 
	int xerr = 0, yerr = 0, delta_x, delta_y, distance; 
	int incx, incy, uRow, uCol; 
	delta_x = x2 - x1; //计算坐标增量 
	delta_y = y2 - y1; 
	uRow = x1; 
	uCol = y1; 
	
	if(delta_x > 0)
	{
		incx = 1; //设置单步方向
	}
	else if(delta_x == 0)
	{
		incx = 0;//垂直线 
	}
	else 
	{
		incx = -1;
		delta_x = -delta_x;
	} 
	
	if(delta_y > 0)
	{
		incy = 1; 
	}
	else if(delta_y == 0)
	{
		incy = 0;//水平线 
	}
	else
	{
		incy = -1;
		delta_y = -delta_y;
	} 
	
	if(delta_x > delta_y)
	{
		distance = delta_x; 	//选取基本增量坐标轴 
	}
	else 
	{
		distance = delta_y; 
	}
	
	for(t = 0; t <= distance + 1; t++ )//画线输出 
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
	LCD_DrawLine(x1 , y1 , x2 , y1);
	LCD_DrawLine(x1 , y1 , x1 , y2);
	LCD_DrawLine(x1 , y2 , x2 , y2);
	LCD_DrawLine(x2 , y1 , x2 , y2);
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
	di = 3 - (r<<1);             //判断下个点位置的标志
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
		if(di < 0)
		{
			di += 4 * a + 6;	
		}			
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
static void LCD_ShowChar(INT16U x, INT16U y, INT8U num, INT8U size, INT8U mode)
{  							  
    INT8U temp, t1, t;
	INT16U y0 = y;
	INT8U csize = (size / 8 + ((size % 8) ? 1:0)) * (size / 2);		//得到字体一个字符对应点阵集所占的字节数	
 	num = num - ' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	
	for(t = 0; t < csize; t++)
	{   
		if(size == 12)
		{
			temp = ASCII_1206[num][t]; 	 //调用1206字体
		}
		else if(size == 16)
		{
			temp = ASCII_1608[num][t];	//调用1608字体
		}
		else if(size == 24)
		{
			temp = ASCII_2412[num][t];	//调用2412字体
		}
		else if(size == 32)
		{
			temp = ASCII_3216[num][t];	//调用3216字体
		}
		else return;					//没有的字库
		
		for(t1 = 0; t1 < 8; t1++)
		{			    
			if(temp & 0x80)
			{
				LCD_Fast_DrawPoint(x, y, POINT_COLOR);
			}
			else if(mode == 0)
			{
				LCD_Fast_DrawPoint(x, y, BACK_COLOR);
			}
			temp <<= 1;
			y++;
			
			if(y >= LCD_HEIGHT)
			{
				return;		//超区域了
			}
			
			if((y - y0) == size)
			{
				y = y0;
				x++;
				if(x >= LCD_WIDTH)
				{
					return;	//超区域了
				}
				break;
			}
		}  	 
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


//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
void LCD_ShowString(INT16U x, INT16U y, INT16U width, INT16U height, INT8U size, INT8U *p)
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










/* ------------------------------------------end of file---------------------------------------- */

