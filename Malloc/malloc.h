/**
  ******************************************************************************
  * @file    malloc.h
  * @author  JFY
  * @version V1.0
  * @date    2020-08-08
  ******************************************************************************
  */ 

#ifndef __MALLOC_H
#define __MALLOC_H

#include "includes.h"


//���������ڴ��
#define SRAMIN	 	0		//�ڲ��ڴ��
#define SRAMEX   	1		//�ⲿ�ڴ��
#define SRAMCCM  	2		//CCM�ڴ��

#define SRAMBANK 	3		//����֧�ֵ�SRAM����.	


//mem1�ڴ�����趨.mem1��ȫ�����ڲ�SRAM����.
#define MEM1_BLOCK_SIZE			64  	  						//�ڴ���СΪ64�ֽ�
#define MEM1_MAX_SIZE			50 * 1024  						//�������ڴ� 160K
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE / MEM1_BLOCK_SIZE //�ڴ���С

//mem2�ڴ�����趨.mem2���ڴ�ش����ⲿSDRAM����
#define MEM2_BLOCK_SIZE			64  	  						//�ڴ���СΪ64�ֽ�
#define MEM2_MAX_SIZE			8 * 1024 * 1024  				//�������ڴ�20M
#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE / MEM2_BLOCK_SIZE //�ڴ���С
		 
//mem3�ڴ�����趨.mem3����CCM,���ڹ���CCM(
#define MEM3_BLOCK_SIZE			64  	  						//�ڴ���СΪ64�ֽ�
#define MEM3_MAX_SIZE			20 * 1024  						//�������ڴ�60K
#define MEM3_ALLOC_TABLE_SIZE	MEM3_MAX_SIZE / MEM3_BLOCK_SIZE //�ڴ���С
		 

//�ڴ���������
struct mem_malloc_dev
{
	void 	(*init)(INT8U);		//��ʼ��
	INT16U 	(*perused)(INT8U);	//�ڴ�ʹ����
	INT8U 	*membase[SRAMBANK];	//�ڴ�� 
	INT32U 	*memmap[SRAMBANK]; 	//�ڴ����״̬��
	INT8U   memrdy[SRAMBANK]; 	//�ڴ�������״̬
};
 

//�ڲ����ú���
void mymemset(void *s, INT8U c, INT32U count);	//�����ڴ�
void mymemcpy(void *des, void *src, INT32U n);	//�����ڴ�     
void my_mem_init(INT8U memx);					//�ڴ�����ʼ������(��/�ڲ�����)
INT32U my_mem_malloc(INT8U memx, INT32U size);	//�ڴ����(�ڲ�����)
INT8U  my_mem_free(INT8U memx, INT32U offset);	//�ڴ��ͷ�(�ڲ�����)
INT16U my_mem_perused(INT8U memx) ;				//����ڴ�ʹ����(��/�ڲ�����) 

//�û����ú���
void  myfree(INT8U memx, void *ptr);  			//�ڴ��ͷ�(�ⲿ����)
void *mymalloc(INT8U memx, INT32U size);		//�ڴ����(�ⲿ����)
void *myrealloc(INT8U memx, void *ptr, INT32U size);//���·����ڴ�(�ⲿ����)


#endif













