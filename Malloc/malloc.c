#include "malloc.h"	   


//内存池(32字节对齐)
__align(32) INT8U mem1base[MEM1_MAX_SIZE];													//内部SRAM内存池
__align(32) INT8U mem2base[MEM2_MAX_SIZE] __attribute__((at(0XC0600000)));					//外部SDRAM内存池,前面2M给LTDC用了(1280*800*2)
__align(32) INT8U mem3base[MEM3_MAX_SIZE] __attribute__((at(0X10000000)));					//内部CCM内存池
//内存管理表
INT32U mem1mapbase[MEM1_ALLOC_TABLE_SIZE];													//内部SRAM内存池MAP
INT32U mem2mapbase[MEM2_ALLOC_TABLE_SIZE] __attribute__((at(0XC0600000 + MEM2_MAX_SIZE)));	//外部SRAM内存池MAP
INT32U mem3mapbase[MEM3_ALLOC_TABLE_SIZE] __attribute__((at(0X10000000 + MEM3_MAX_SIZE)));	//内部CCM内存池MAP
//内存管理参数	   
const INT32U memtblsize[SRAMBANK] = {MEM1_ALLOC_TABLE_SIZE, MEM2_ALLOC_TABLE_SIZE, MEM3_ALLOC_TABLE_SIZE};	//内存表大小
const INT32U memblksize[SRAMBANK] = {MEM1_BLOCK_SIZE, MEM2_BLOCK_SIZE, MEM3_BLOCK_SIZE};					//内存分块大小
const INT32U memsize[SRAMBANK] = {MEM1_MAX_SIZE, MEM2_MAX_SIZE, MEM3_MAX_SIZE};								//内存总大小

//内存管理控制器
struct mem_malloc_dev malloc_dev =
{
	my_mem_init,							//内存初始化
	my_mem_perused,							//内存使用率
	mem1base, mem2base, mem3base,			//内存池
	mem1mapbase, mem2mapbase, mem3mapbase,	//内存管理状态表
	0, 0, 0,  		 						//内存管理未就绪
};


//复制内存
//*des:目的地址
//*src:源地址
//n:需要复制的内存长度(字节为单位)
void mymemcpy(void *des, void *src, INT32U n)  
{  
    INT8U *xdes = des;
	INT8U *xsrc = src; 
    while(n--)
	{
		*xdes++ = *xsrc++; 
	}		
} 


//设置内存
//*s:内存首地址
//c :要设置的值
//count:需要设置的内存大小(字节为单位)
void mymemset(void *s, INT8U c, INT32U count)  
{  
    INT8U *xs = s;  
	
    while(count--)
	{
		*xs++ = c;  
	}
}	


//内存管理初始化  
//memx:所属内存块
void my_mem_init(INT8U memx)  
{  
    mymemset(malloc_dev.memmap[memx], 0, memtblsize[memx] * 4);	//内存状态表数据清零  
 	malloc_dev.memrdy[memx] = 1;								//内存管理初始化OK  
} 


//获取内存使用率
//memx:所属内存块
//返回值:使用率(扩大了10倍,0~1000,代表0.0%~100.0%)
INT16U my_mem_perused(INT8U memx)  
{  
    INT32U used = 0;  
    INT32U i;  
    for(i = 0; i < memtblsize[memx]; i++)  
    {  
        if(malloc_dev.memmap[memx][i])
		{
			used++; 
		}
    } 
    return (used * 1000) / (memtblsize[memx]);  
}  


//内存分配(内部调用)
//memx:所属内存块
//size:要分配的内存大小(字节)
//返回值:0XFFFFFFFF,代表错误;其他,内存偏移地址 
INT32U my_mem_malloc(INT8U memx, INT32U size)  
{  
    signed long offset = 0;  
    INT32U nmemb;  		//需要的内存块数  
	INT32U cmemb = 0;	//连续空内存块数
    INT32U i; 
	
    if(!malloc_dev.memrdy[memx])
	{
		malloc_dev.init(memx);//初始化内存状态表
	}
	
    if(size == 0)
	{
		return 0XFFFFFFFF;//不需要分配
	}
	
    nmemb = size / memblksize[memx];  	//分配连续内存块数
	
    if(size % memblksize[memx]) //分配的内存块加1
	{
		nmemb++;  
	}
	
    for(offset = memtblsize[memx]-1; offset >= 0; offset--)//搜索整个内存控制区  
    {     
		if(!malloc_dev.memmap[memx][offset]) //对内存状态表进行判断（内存使用情况）
		{
			cmemb++;//连续空内存块数增加
		}
		else 
		{
			cmemb = 0;		//连续内存块清零
		}
		
		if(cmemb == nmemb)	//找到了连续nmemb个空内存块
		{
            for(i = 0; i < nmemb; i++)  //标注内存块非空 
            {  
                malloc_dev.memmap[memx][offset + i] = nmemb;  
            }  
            return (offset * memblksize[memx]);//返回偏移地址  
		}
    }  
    return 0XFFFFFFFF;//未找到符合分配条件的内存块  
}  


//释放内存(内部调用) 
//memx:所属内存块
//offset:内存地址偏移
//返回值:0,释放成功;1,释放失败;  
INT8U my_mem_free(INT8U memx, INT32U offset)  
{  
    int i;  
	
    if(!malloc_dev.memrdy[memx])//未初始化,先执行初始化
	{
		malloc_dev.init(memx);    
        return 1;				//未初始化  
    } 
	
    if(offset < memsize[memx])  //偏移在内存池内. 
    {  
        int index = offset / memblksize[memx];		//偏移所在内存块号码  
        int nmemb = malloc_dev.memmap[memx][index];	//内存块数量
        for(i = 0; i < nmemb; i++)  				//内存块清零
        {  
            malloc_dev.memmap[memx][index + i] = 0;  
        }  
        return 0;  
    }
	else 
	{
		return 2;//偏移超区了. 
	}		
}  


//释放内存(外部调用) 
//memx:所属内存块
//ptr:内存首地址 
void myfree(INT8U memx, void *ptr)  
{  
	INT32U offset; 
	
	if(ptr == NULL)
	{
		return;//地址为0.
	}
	//求取偏移地址
 	offset = (INT32U)ptr - (INT32U)malloc_dev.membase[memx];     
    my_mem_free(memx , offset);	//释放内存      
} 


//分配内存(外部调用)
//memx:所属内存块
//size:内存大小(字节)
//返回值:分配到的内存首地址.
void *mymalloc(INT8U memx, INT32U size)  
{  
    INT32U offset;   
	//获取偏移地址
	offset = my_mem_malloc(memx, size);  	   	 	   
    if(offset == 0XFFFFFFFF)
	{
		return NULL;  
	}
    else 
	{
		return (void*)((INT32U)malloc_dev.membase[memx] + offset);  
	}
}  


//重新分配内存(外部调用)
//memx:所属内存块
//*ptr:旧内存首地址
//size:要分配的内存大小(字节)
//返回值:新分配到的内存首地址.
void *myrealloc(INT8U memx, void *ptr, INT32U size)  
{  
    INT32U offset;    
    offset = my_mem_malloc(memx, size);
   	
    if(offset == 0XFFFFFFFF)
	{
		return NULL; 
	}		
    else  
    {  									   
	    mymemcpy((void*)((INT32U)malloc_dev.membase[memx] + offset), ptr, size);	//拷贝旧内存内容到新内存   
        myfree(memx, ptr);  											  			//释放旧内存
        return (void*)((INT32U)malloc_dev.membase[memx] + offset);  				//返回新内存首地址
    }  
}



