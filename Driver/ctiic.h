#ifndef __MYCT_IIC_H
#define __MYCT_IIC_H

#include "includes.h"

#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

#define GPIOH_ODR_Addr    (GPIOH_BASE + 20) //0x40021C14 
#define GPIOH_IDR_Addr    (GPIOH_BASE + 16) //0x40021C10 
#define GPIOI_ODR_Addr    (GPIOI_BASE + 20) //0x40022014 
#define GPIOI_IDR_Addr    (GPIOI_BASE + 16) //0x40022010 

//IO��������
#define CT_SDA_IN()  {GPIOI->MODER &= ~(3 << (3 * 2)); GPIOI->MODER |= 0 << 3 * 2;}	//PI3����ģʽ
#define CT_SDA_OUT() {GPIOI->MODER &= ~(3 << (3 * 2)); GPIOI->MODER |= 1 << 3 * 2;} //PI3���ģʽ


//IO��������	 
#define CT_IIC_SCL  BIT_ADDR(GPIOH_ODR_Addr, 6)  //��� SCL
#define CT_IIC_SDA  BIT_ADDR(GPIOI_ODR_Addr, 3)  //��� SDA	 
#define CT_READ_SDA BIT_ADDR(GPIOI_IDR_Addr, 3)  //���� SDA 

#define FT_RST  BIT_ADDR(GPIOI_ODR_Addr, 8)      //FT5206��λ����
#define FT_INT  BIT_ADDR(GPIOH_IDR_Addr, 7)      //FT5206�ж�����	
 

//IIC���в�������
void CT_IIC_Init(void);                	//��ʼ��IIC��IO��				 
void CT_IIC_Start(void);				//����IIC��ʼ�ź�
void CT_IIC_Stop(void);	  				//����IICֹͣ�ź�
void CT_IIC_Send_Byte(INT8U txd);		//IIC����һ���ֽ�
INT8U CT_IIC_Read_Byte(INT8U ack);		//IIC��ȡһ���ֽ�
INT8U CT_IIC_Wait_Ack(void); 			//IIC�ȴ�ACK�ź�
void CT_IIC_Ack(void);					//IIC����ACK�ź�
void CT_IIC_NAck(void);					//IIC������ACK�ź�

#endif







