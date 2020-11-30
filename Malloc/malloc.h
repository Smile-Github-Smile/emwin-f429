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


//定义三个内存池
#define SRAMIN	 	0		//内部内存池
#define SRAMEX   	1		//外部内存池
#define SRAMCCM  	2		//CCM内存池

#define SRAMBANK 	3		//定义支持的SRAM块数.	


//mem1内存参数设定.mem1完全处于内部SRAM里面.
#define MEM1_BLOCK_SIZE			64  	  						//内存块大小为64字节
#define MEM1_MAX_SIZE			50 * 1024  						//最大管理内存 160K
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE / MEM1_BLOCK_SIZE //内存表大小

//mem2内存参数设定.mem2的内存池处于外部SDRAM里面
#define MEM2_BLOCK_SIZE			64  	  						//内存块大小为64字节
#define MEM2_MAX_SIZE			8 * 1024 * 1024  				//最大管理内存20M
#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE / MEM2_BLOCK_SIZE //内存表大小
		 
//mem3内存参数设定.mem3处于CCM,用于管理CCM(
#define MEM3_BLOCK_SIZE			64  	  						//内存块大小为64字节
#define MEM3_MAX_SIZE			20 * 1024  						//最大管理内存60K
#define MEM3_ALLOC_TABLE_SIZE	MEM3_MAX_SIZE / MEM3_BLOCK_SIZE //内存表大小
		 

//内存管理控制器
struct mem_malloc_dev
{
	void 	(*init)(INT8U);		//初始化
	INT16U 	(*perused)(INT8U);	//内存使用率
	INT8U 	*membase[SRAMBANK];	//内存池 
	INT32U 	*memmap[SRAMBANK]; 	//内存管理状态表
	INT8U   memrdy[SRAMBANK]; 	//内存管理就绪状态
};
 

//内部调用函数
void mymemset(void *s, INT8U c, INT32U count);	//设置内存
void mymemcpy(void *des, void *src, INT32U n);	//复制内存     
void my_mem_init(INT8U memx);					//内存管理初始化函数(外/内部调用)
INT32U my_mem_malloc(INT8U memx, INT32U size);	//内存分配(内部调用)
INT8U  my_mem_free(INT8U memx, INT32U offset);	//内存释放(内部调用)
INT16U my_mem_perused(INT8U memx) ;				//获得内存使用率(外/内部调用) 

//用户调用函数
void  myfree(INT8U memx, void *ptr);  			//内存释放(外部调用)
void *mymalloc(INT8U memx, INT32U size);		//内存分配(外部调用)
void *myrealloc(INT8U memx, void *ptr, INT32U size);//重新分配内存(外部调用)


#endif













