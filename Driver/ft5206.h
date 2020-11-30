/**
  ******************************************************************************
  * @file    ft5206.h
  * @author  JFY
  * @version V1.0
  * @date    2020-08-08
  ******************************************************************************
  */ 

#ifndef __FT5206_H
#define __FT5206_H	

#include "includes.h"

   
//I2C��д����	
#define FT_CMD_WR 				0X70    	//д����
#define FT_CMD_RD 				0X71		//������
  
//FT5206 ���ּĴ������� 
#define FT_DEVIDE_MODE 			0x00   		//FT5206ģʽ���ƼĴ���
#define FT_REG_NUM_FINGER       0x02		//����״̬�Ĵ���

#define FT_TP1_REG 				0X03	  	//��һ�����������ݵ�ַ
#define FT_TP2_REG 				0X09		//�ڶ������������ݵ�ַ
#define FT_TP3_REG 				0X0F		//���������������ݵ�ַ
#define FT_TP4_REG 				0X15		//���ĸ����������ݵ�ַ
#define FT_TP5_REG 				0X1B		//��������������ݵ�ַ  
 

#define	FT_ID_G_LIB_VERSION		0xA1		//�汾		
#define FT_ID_G_MODE 			0xA4   		//FT5206�ж�ģʽ���ƼĴ���
#define FT_ID_G_THGROUP			0x80   		//������Чֵ���üĴ���
#define FT_ID_G_PERIODACTIVE	0x88   		//����״̬�������üĴ���


#define TP_PRES_DOWN 			0x80  		//����������	  
#define TP_CATH_PRES 			0x40  		//�а��������� 
#define CT_MAX_TOUCH  			5    		//������֧�ֵĵ���,�̶�Ϊ5��

//������������
typedef struct
{
	INT8U (*init)(void);		//��ʼ��������������
	INT8U (*scan)(INT8U);		//ɨ�败����.0,��Ļɨ��;1,��������;	 
	INT16U x[CT_MAX_TOUCH]; 	//��ǰ����
	INT16U y[CT_MAX_TOUCH];		//�����������5������,����������x[0],y[0]����:�˴�ɨ��ʱ,����������,��
								//x[4],y[4]�洢��һ�ΰ���ʱ������. 
	INT8U  sta;					//�ʵ�״̬ 
								//b7:����1/�ɿ�0; 
	                            //b6:0,û�а�������;1,�а�������. 
								//b5:����
								//b4~b0:���ݴ��������µĵ���(0,��ʾδ����,1��ʾ����)
   
	INT8U touchtype;
}tpdev_struct;


extern tpdev_struct tp_dev;


INT8U FT5206_WR_Reg(INT16U reg,INT8U *buf,INT8U len);
void FT5206_RD_Reg(INT16U reg,INT8U *buf,INT8U len);
INT8U FT5206_Init(void);
INT8U FT5206_Scan(INT8U mode);

#endif

















