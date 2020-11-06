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



//����
//x,y:����
//POINT_COLOR:�˵����ɫ
static void LCD_DrawPoint(INT16U x, INT16U y)
{
	LTDC_Draw_Point(x, y, POINT_COLOR);
}

//���ٻ���
//x,y:����
//color:��ɫ
void LCD_Fast_DrawPoint(INT16U x, INT16U y, INT32U color)
{	   
	LTDC_Draw_Point(x, y, color);
}

//��ȡ��ĳ�����ɫֵ	 
//x,y:����
//����ֵ:�˵����ɫ
INT32U LCD_ReadPoint(INT16U x, INT16U y)
{
	return LTDC_Read_Point(x, y);
}

//��ָ����������䵥����ɫ
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void LCD_Color_Fill(INT16U sx, INT16U sy, INT16U ex, INT16U ey, INT32U color)
{          
	LTDC_Color_Fill(sx, sy, ex, ey, color);
} 


//����
//x1,y1:�������
//x2,y2:�յ�����  
void LCD_DrawLine(INT16U x1, INT16U y1, INT16U x2, INT16U y2)
{
	INT16U t; 
	int xerr = 0, yerr = 0, delta_x, delta_y, distance; 
	int incx, incy, uRow, uCol; 
	delta_x = x2 - x1; //������������ 
	delta_y = y2 - y1; 
	uRow = x1; 
	uCol = y1; 
	
	if(delta_x > 0)
	{
		incx = 1; //���õ�������
	}
	else if(delta_x == 0)
	{
		incx = 0;//��ֱ�� 
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
		incy = 0;//ˮƽ�� 
	}
	else
	{
		incy = -1;
		delta_y = -delta_y;
	} 
	
	if(delta_x > delta_y)
	{
		distance = delta_x; 	//ѡȡ�������������� 
	}
	else 
	{
		distance = delta_y; 
	}
	
	for(t = 0; t <= distance + 1; t++ )//������� 
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
	LCD_DrawLine(x1 , y1 , x2 , y1);
	LCD_DrawLine(x1 , y1 , x1 , y2);
	LCD_DrawLine(x1 , y2 , x2 , y2);
	LCD_DrawLine(x2 , y1 , x2 , y2);
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
	di = 3 - (r<<1);             //�ж��¸���λ�õı�־
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

//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16/24/32
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
static void LCD_ShowChar(INT16U x, INT16U y, INT8U num, INT8U size, INT8U mode)
{  							  
    INT8U temp, t1, t;
	INT16U y0 = y;
	INT8U csize = (size / 8 + ((size % 8) ? 1:0)) * (size / 2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���	
 	num = num - ' ';//�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
	
	for(t = 0; t < csize; t++)
	{   
		if(size == 12)
		{
			temp = ASCII_1206[num][t]; 	 //����1206����
		}
		else if(size == 16)
		{
			temp = ASCII_1608[num][t];	//����1608����
		}
		else if(size == 24)
		{
			temp = ASCII_2412[num][t];	//����2412����
		}
		else if(size == 32)
		{
			temp = ASCII_3216[num][t];	//����3216����
		}
		else return;					//û�е��ֿ�
		
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
				return;		//��������
			}
			
			if((y - y0) == size)
			{
				y = y0;
				x++;
				if(x >= LCD_WIDTH)
				{
					return;	//��������
				}
				break;
			}
		}  	 
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


//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void LCD_ShowString(INT16U x, INT16U y, INT16U width, INT16U height, INT8U size, INT8U *p)
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










/* ------------------------------------------end of file---------------------------------------- */

