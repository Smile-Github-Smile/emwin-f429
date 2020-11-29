#include "ctiic.h"
#include "stm32f4xx.h"  
 
//����ʱ����
static void delay_us(INT32U time)
{
	while(time--)
	{
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	}
}

//���ݴ���оƬIIC�ӿڳ�ʼ��
void CT_IIC_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  
	GPIO_Init(GPIOH, &GPIO_InitStructure); 		//T_SCK/SCL
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  
	GPIO_Init(GPIOI, &GPIO_InitStructure); 		//T_MOSI/SDA	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;  
	GPIO_Init(GPIOI, &GPIO_InitStructure); 		//T_CS/RST 
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;  
	GPIO_Init(GPIOH, &GPIO_InitStructure); 		//T_PEN/�ж����� 
    CT_IIC_SDA = 1;	  	  
	CT_IIC_SCL = 1;
}


//����IIC��ʼ�ź�
void CT_IIC_Start(void)
{
	CT_SDA_OUT();   //sda�����
	CT_IIC_SDA = 1;	  	  
	CT_IIC_SCL = 1;
	delay_us(30);
 	CT_IIC_SDA = 0;	//START:when CLK is high,DATA change form high to low 
	delay_us(2);
	CT_IIC_SCL = 0;	//ǯסI2C���ߣ�׼�����ͻ�������� 
}	


//����IICֹͣ�ź�
void CT_IIC_Stop(void)
{
	CT_SDA_OUT();	//sda�����
	CT_IIC_SCL = 0; 
	CT_IIC_SDA = 0;	//STOP:when CLK is high DATA change form low to high
	delay_us(30);
	CT_IIC_SCL = 1; 
	delay_us(2); 
	CT_IIC_SDA = 1;	//����I2C���߽����ź�  
}


//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
INT8U CT_IIC_Wait_Ack(void)
{
	INT8U ucErrTime = 0;
	CT_SDA_IN();      //SDA����Ϊ����  
	CT_IIC_SDA = 1;	   
	CT_IIC_SCL = 1;
	delay_us(2);
	while(CT_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime > 250)
		{
			CT_IIC_Stop();
			return 1;
		} 
		delay_us(2);
	}
	CT_IIC_SCL = 0;	//ʱ�����0 	   
	return 0;  
} 


//����ACKӦ��
void CT_IIC_Ack(void)
{
	CT_IIC_SCL = 0;
	CT_SDA_OUT(); 
	CT_IIC_SDA = 0;
	delay_us(2);
	CT_IIC_SCL = 1;
	delay_us(2);
	CT_IIC_SCL = 0;
}


//������ACKӦ��		    
void CT_IIC_NAck(void)
{
	CT_IIC_SCL = 0;
	CT_SDA_OUT(); 
	CT_IIC_SDA = 1;
	delay_us(2);
	CT_IIC_SCL = 1;
	delay_us(2);
	CT_IIC_SCL = 0;
}


//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void CT_IIC_Send_Byte(INT8U txd)
{                        
    INT8U t;   
	CT_SDA_OUT(); 	    
    CT_IIC_SCL = 0;//����ʱ�ӿ�ʼ���ݴ���
	delay_us(2);
	for(t = 0; t < 8; t++)
    {              
        CT_IIC_SDA = (txd & 0x80) >> 7;
        txd <<= 1; 	
        delay_us(2);        
		CT_IIC_SCL = 1; 
		delay_us(2);
		CT_IIC_SCL = 0;	
		delay_us(2);
    }	 
} 


//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
INT8U CT_IIC_Read_Byte(INT8U ack)
{
	INT8U i,receive = 0;
 	CT_SDA_IN();		//SDA����Ϊ����
	delay_us(30);
	for(i = 0; i < 8; i++ )
	{ 
		CT_IIC_SCL = 0; 	    	   
		delay_us(2);
		CT_IIC_SCL = 1;	 
		receive <<= 1;
		if(CT_READ_SDA)
		{
			receive++;  
		}			
        delay_us(2); 
	}	  				 
	if (!ack)
	{
		CT_IIC_NAck();	//����nACK
	}
	else 
	{
		CT_IIC_Ack(); 	//����ACK 
	}		
 	return receive;
}





