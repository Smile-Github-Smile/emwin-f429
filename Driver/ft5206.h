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

   
//I2C读写命令	
#define FT_CMD_WR 				0X70    	//写命令
#define FT_CMD_RD 				0X71		//读命令
  
//FT5206 部分寄存器定义 
#define FT_DEVIDE_MODE 			0x00   		//FT5206模式控制寄存器
#define FT_REG_NUM_FINGER       0x02		//触摸状态寄存器

#define FT_TP1_REG 				0X03	  	//第一个触摸点数据地址
#define FT_TP2_REG 				0X09		//第二个触摸点数据地址
#define FT_TP3_REG 				0X0F		//第三个触摸点数据地址
#define FT_TP4_REG 				0X15		//第四个触摸点数据地址
#define FT_TP5_REG 				0X1B		//第五个触摸点数据地址  
 

#define	FT_ID_G_LIB_VERSION		0xA1		//版本		
#define FT_ID_G_MODE 			0xA4   		//FT5206中断模式控制寄存器
#define FT_ID_G_THGROUP			0x80   		//触摸有效值设置寄存器
#define FT_ID_G_PERIODACTIVE	0x88   		//激活状态周期设置寄存器


#define TP_PRES_DOWN 			0x80  		//触屏被按下	  
#define TP_CATH_PRES 			0x40  		//有按键按下了 
#define CT_MAX_TOUCH  			5    		//电容屏支持的点数,固定为5点

//触摸屏控制器
typedef struct
{
	INT8U (*init)(void);		//初始化触摸屏控制器
	INT8U (*scan)(INT8U);		//扫描触摸屏.0,屏幕扫描;1,物理坐标;	 
	INT16U x[CT_MAX_TOUCH]; 	//当前坐标
	INT16U y[CT_MAX_TOUCH];		//电容屏有最多5组坐标,电阻屏则用x[0],y[0]代表:此次扫描时,触屏的坐标,用
								//x[4],y[4]存储第一次按下时的坐标. 
	INT8U  sta;					//笔的状态 
								//b7:按下1/松开0; 
	                            //b6:0,没有按键按下;1,有按键按下. 
								//b5:保留
								//b4~b0:电容触摸屏按下的点数(0,表示未按下,1表示按下)
   
	INT8U touchtype;
}tpdev_struct;


extern tpdev_struct tp_dev;


INT8U FT5206_WR_Reg(INT16U reg,INT8U *buf,INT8U len);
void FT5206_RD_Reg(INT16U reg,INT8U *buf,INT8U len);
INT8U FT5206_Init(void);
INT8U FT5206_Scan(INT8U mode);

#endif

















