/**
  ******************************************************************************
  * @file    ft5206.c
  * @author  JFY
  * @version V1.0
  * @date    2020-08-08
  ******************************************************************************
  */ 
#include "ft5206.h"
#include "ctiic.h"
#include "tftlcd.h"


const INT16U FT5206_TPX_TBL[5] = {FT_TP1_REG, FT_TP2_REG, FT_TP3_REG, FT_TP4_REG, FT_TP5_REG};


static void FT5206_RD_Reg(INT16U reg, INT8U *buf, INT8U len);
static INT8U FT5206_WR_Reg(INT16U reg, INT8U *buf, INT8U len);
static INT8U FT5206_Init(void);
static INT8U FT5206_Scan(INT8U mode);
static INT8U TP_Init(void);


tpdev_struct tp_dev =
{
	TP_Init,
	FT5206_Scan,
	0,
	0, 
	0,
	0,  	 		
};


//��������ʼ��  		    
//����ֵ:0,û�н���У׼
//       1,���й�У׼
static INT8U TP_Init(void)
{	
	FT5206_Init();
	tp_dev.scan = FT5206_Scan;				//ɨ�躯��ָ��GT9147������ɨ��		
	tp_dev.touchtype |= 0X80;				//������ 
	tp_dev.touchtype |= lcddev.dir & 0X01;	//������������ 
	return 0;								 
}

//��FT5206д��һ������
//reg:��ʼ�Ĵ�����ַ
//buf:���ݻ�������
//len:д���ݳ���
//����ֵ:0,�ɹ�;1,ʧ��.
static INT8U FT5206_WR_Reg(INT16U reg, INT8U *buf, INT8U len)
{
	INT8U i;
	INT8U ret = 0;
	CT_IIC_Start();	 
	CT_IIC_Send_Byte(FT_CMD_WR);	//����д���� 	 
	CT_IIC_Wait_Ack(); 	 										  		   
	CT_IIC_Send_Byte(reg & 0XFF);   //���͵�8λ��ַ
	CT_IIC_Wait_Ack();  
	for(i = 0; i < len; i++)
	{	   
    	CT_IIC_Send_Byte(buf[i]);  	//������
		ret = CT_IIC_Wait_Ack();
		if(ret)
		{
			break;  
		}
	}
    CT_IIC_Stop();					//����һ��ֹͣ����	    
	return ret; 
}


//��FT5206����һ������
//reg:��ʼ�Ĵ�����ַ
//buf:���ݻ�������
//len:�����ݳ���			  
static void FT5206_RD_Reg(INT16U reg, INT8U *buf, INT8U len)
{
	INT8U i; 
 	CT_IIC_Start();	
 	CT_IIC_Send_Byte(FT_CMD_WR);   	//����д���� 	 
	CT_IIC_Wait_Ack(); 	 										  		   
 	CT_IIC_Send_Byte(reg & 0XFF);   //���͵�8λ��ַ
	CT_IIC_Wait_Ack();  
 	CT_IIC_Start();  	 	   
	CT_IIC_Send_Byte(FT_CMD_RD);   	//���Ͷ�����		   
	CT_IIC_Wait_Ack();	   
	for(i = 0; i < len; i++)
	{	   
    	buf[i] = CT_IIC_Read_Byte(i == (len-1) ? 0 : 1); //������	  
	} 
    CT_IIC_Stop();//����һ��ֹͣ����     
} 


//��ʼ��FT5206������
//����ֵ:0,��ʼ���ɹ�;1,��ʼ��ʧ�� 
static INT8U FT5206_Init(void)
{
	INT16U ii;
	INT8U temp[2];    
	CT_IIC_Init();      	//��ʼ�� 
	FT_RST = 0;				//��λ
	for(ii = 0; ii < 20000; ii++)
	{}
 	FT_RST = 1;				//�ͷŸ�λ		    
	for(ii = 0; ii < 40000; ii++)
	{} 	
	temp[0] = 0;
	FT5206_WR_Reg(FT_DEVIDE_MODE , temp , 1);	//������������ģʽ 
	FT5206_WR_Reg(FT_ID_G_MODE , temp , 1);		//��ѯģʽ 
	temp[0] = 22;								//������Чֵ��22��ԽСԽ����	
	FT5206_WR_Reg(FT_ID_G_THGROUP , temp , 1);	//���ô�����Чֵ
	temp[0] = 12;								//�������ڣ�����С��12�����14
	FT5206_WR_Reg(FT_ID_G_PERIODACTIVE , temp , 1); 
	//��ȡ�汾�ţ��ο�ֵ��0x3003
	FT5206_RD_Reg(FT_ID_G_LIB_VERSION , &temp[0] , 2);  
	if((temp[0] == 0X30 && temp[1] == 0X03) || (temp[1] == 0X01 || temp[1] == 0X02))//�汾:0X3003/0X0001/0X0002
	{
		//printf("CTP ID:%x\r\n",((INT16U)temp[0] << 8) + temp[1]);
		return 0;
	} 
	return 1;
}



//ɨ�败����(���ò�ѯ��ʽ)
//mode:0,����ɨ��.
//����ֵ:��ǰ����״̬.
//0,�����޴���;1,�����д���
static INT8U FT5206_Scan(INT8U mode)
{
	INT8U buf[4];
	INT8U i = 0;
	INT8U res = 0;
	INT8U temp;
	static INT8U t = 0;//���Ʋ�ѯ���,�Ӷ�����CPUռ����   
	t++;
	if(((t % 10) == 0) || (t < 10))//����ʱ,ÿ����10��CTP_Scan�����ż��1��,�Ӷ���ʡCPUʹ����
	{
		FT5206_RD_Reg(FT_REG_NUM_FINGER, &mode, 1);//��ȡ�������״̬  
		if((mode & 0XF) && ((mode & 0XF) < 6))
		{
			temp = 0XFF << (mode & 0XF);//����ĸ���ת��Ϊ1��λ��,ƥ��tp_dev.sta���� 
			tp_dev.sta = (~temp) | TP_PRES_DOWN | TP_CATH_PRES; 
			for(i = 0; i < 5; i++)
			{
				if(tp_dev.sta & (1 << i))	//������Ч?
				{
					FT5206_RD_Reg(FT5206_TPX_TBL[i], buf, 4);	//��ȡXY����ֵ 
					if(tp_dev.touchtype & 0X01)//����
					{
						tp_dev.y[i] = ((INT16U)(buf[0] & 0X0F) << 8) + buf[1];
						tp_dev.x[i] = ((INT16U)(buf[2] & 0X0F) << 8) + buf[3];
					}else
					{
						tp_dev.x[i] = lcddev.width - (((INT16U)(buf[0] & 0X0F) << 8) + buf[1]);
						tp_dev.y[i] = ((INT16U)(buf[2] & 0X0F) << 8) + buf[3];
					}  
					if((buf[0] & 0XF0) != 0X80)
					{
						tp_dev.x[i] = tp_dev.y[i] = 0;//������contact�¼�������Ϊ��Ч
					}
					//printf("x[%d]:%d,y[%d]:%d\r\n", i, tp_dev.x[i], i, tp_dev.y[i]);
				}			
			} 
			res = 1;
			if((tp_dev.x[0] == 0) && (tp_dev.y[0] == 0))
			{
				mode = 0;	//���������ݶ���0,����Դ˴�����
			}
			t = 0;		//����һ��,��������������10��,�Ӷ����������
		}
	}
	if((mode & 0X1F) == 0)//�޴����㰴��
	{ 
		if(tp_dev.sta & TP_PRES_DOWN)	//֮ǰ�Ǳ����µ�
		{
			tp_dev.sta &= ~(1 << 7);	//��ǰ����ɿ�
		}else						//֮ǰ��û�б�����
		{ 
			tp_dev.x[0] = 0xffff;
			tp_dev.y[0] = 0xffff;
			tp_dev.sta &= 0XE0;	//�������Ч���	
		}	 
	} 	
	if(t > 240)
	{
		t = 10;//���´�10��ʼ����
	}
	return res;
}
 

