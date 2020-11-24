#include "malloc.h"	   


//�ڴ��(32�ֽڶ���)
__align(32) INT8U mem1base[MEM1_MAX_SIZE];													//�ڲ�SRAM�ڴ��
__align(32) INT8U mem2base[MEM2_MAX_SIZE] __attribute__((at(0XC0600000)));					//�ⲿSDRAM�ڴ��,ǰ��2M��LTDC����(1280*800*2)
__align(32) INT8U mem3base[MEM3_MAX_SIZE] __attribute__((at(0X10000000)));					//�ڲ�CCM�ڴ��
//�ڴ�����
INT32U mem1mapbase[MEM1_ALLOC_TABLE_SIZE];													//�ڲ�SRAM�ڴ��MAP
INT32U mem2mapbase[MEM2_ALLOC_TABLE_SIZE] __attribute__((at(0XC0600000 + MEM2_MAX_SIZE)));	//�ⲿSRAM�ڴ��MAP
INT32U mem3mapbase[MEM3_ALLOC_TABLE_SIZE] __attribute__((at(0X10000000 + MEM3_MAX_SIZE)));	//�ڲ�CCM�ڴ��MAP
//�ڴ�������	   
const INT32U memtblsize[SRAMBANK] = {MEM1_ALLOC_TABLE_SIZE, MEM2_ALLOC_TABLE_SIZE, MEM3_ALLOC_TABLE_SIZE};	//�ڴ���С
const INT32U memblksize[SRAMBANK] = {MEM1_BLOCK_SIZE, MEM2_BLOCK_SIZE, MEM3_BLOCK_SIZE};					//�ڴ�ֿ��С
const INT32U memsize[SRAMBANK] = {MEM1_MAX_SIZE, MEM2_MAX_SIZE, MEM3_MAX_SIZE};								//�ڴ��ܴ�С

//�ڴ���������
struct mem_malloc_dev malloc_dev =
{
	my_mem_init,							//�ڴ��ʼ��
	my_mem_perused,							//�ڴ�ʹ����
	mem1base, mem2base, mem3base,			//�ڴ��
	mem1mapbase, mem2mapbase, mem3mapbase,	//�ڴ����״̬��
	0, 0, 0,  		 						//�ڴ����δ����
};


//�����ڴ�
//*des:Ŀ�ĵ�ַ
//*src:Դ��ַ
//n:��Ҫ���Ƶ��ڴ泤��(�ֽ�Ϊ��λ)
void mymemcpy(void *des, void *src, INT32U n)  
{  
    INT8U *xdes = des;
	INT8U *xsrc = src; 
    while(n--)
	{
		*xdes++ = *xsrc++; 
	}		
} 


//�����ڴ�
//*s:�ڴ��׵�ַ
//c :Ҫ���õ�ֵ
//count:��Ҫ���õ��ڴ��С(�ֽ�Ϊ��λ)
void mymemset(void *s, INT8U c, INT32U count)  
{  
    INT8U *xs = s;  
	
    while(count--)
	{
		*xs++ = c;  
	}
}	


//�ڴ�����ʼ��  
//memx:�����ڴ��
void my_mem_init(INT8U memx)  
{  
    mymemset(malloc_dev.memmap[memx], 0, memtblsize[memx] * 4);	//�ڴ�״̬����������  
 	malloc_dev.memrdy[memx] = 1;								//�ڴ�����ʼ��OK  
} 


//��ȡ�ڴ�ʹ����
//memx:�����ڴ��
//����ֵ:ʹ����(������10��,0~1000,����0.0%~100.0%)
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


//�ڴ����(�ڲ�����)
//memx:�����ڴ��
//size:Ҫ������ڴ��С(�ֽ�)
//����ֵ:0XFFFFFFFF,�������;����,�ڴ�ƫ�Ƶ�ַ 
INT32U my_mem_malloc(INT8U memx, INT32U size)  
{  
    signed long offset = 0;  
    INT32U nmemb;  		//��Ҫ���ڴ����  
	INT32U cmemb = 0;	//�������ڴ����
    INT32U i; 
	
    if(!malloc_dev.memrdy[memx])
	{
		malloc_dev.init(memx);//��ʼ���ڴ�״̬��
	}
	
    if(size == 0)
	{
		return 0XFFFFFFFF;//����Ҫ����
	}
	
    nmemb = size / memblksize[memx];  	//���������ڴ����
	
    if(size % memblksize[memx]) //������ڴ���1
	{
		nmemb++;  
	}
	
    for(offset = memtblsize[memx]-1; offset >= 0; offset--)//���������ڴ������  
    {     
		if(!malloc_dev.memmap[memx][offset]) //���ڴ�״̬������жϣ��ڴ�ʹ�������
		{
			cmemb++;//�������ڴ��������
		}
		else 
		{
			cmemb = 0;		//�����ڴ������
		}
		
		if(cmemb == nmemb)	//�ҵ�������nmemb�����ڴ��
		{
            for(i = 0; i < nmemb; i++)  //��ע�ڴ��ǿ� 
            {  
                malloc_dev.memmap[memx][offset + i] = nmemb;  
            }  
            return (offset * memblksize[memx]);//����ƫ�Ƶ�ַ  
		}
    }  
    return 0XFFFFFFFF;//δ�ҵ����Ϸ����������ڴ��  
}  


//�ͷ��ڴ�(�ڲ�����) 
//memx:�����ڴ��
//offset:�ڴ��ַƫ��
//����ֵ:0,�ͷųɹ�;1,�ͷ�ʧ��;  
INT8U my_mem_free(INT8U memx, INT32U offset)  
{  
    int i;  
	
    if(!malloc_dev.memrdy[memx])//δ��ʼ��,��ִ�г�ʼ��
	{
		malloc_dev.init(memx);    
        return 1;				//δ��ʼ��  
    } 
	
    if(offset < memsize[memx])  //ƫ�����ڴ����. 
    {  
        int index = offset / memblksize[memx];		//ƫ�������ڴ�����  
        int nmemb = malloc_dev.memmap[memx][index];	//�ڴ������
        for(i = 0; i < nmemb; i++)  				//�ڴ������
        {  
            malloc_dev.memmap[memx][index + i] = 0;  
        }  
        return 0;  
    }
	else 
	{
		return 2;//ƫ�Ƴ�����. 
	}		
}  


//�ͷ��ڴ�(�ⲿ����) 
//memx:�����ڴ��
//ptr:�ڴ��׵�ַ 
void myfree(INT8U memx, void *ptr)  
{  
	INT32U offset; 
	
	if(ptr == NULL)
	{
		return;//��ַΪ0.
	}
	//��ȡƫ�Ƶ�ַ
 	offset = (INT32U)ptr - (INT32U)malloc_dev.membase[memx];     
    my_mem_free(memx , offset);	//�ͷ��ڴ�      
} 


//�����ڴ�(�ⲿ����)
//memx:�����ڴ��
//size:�ڴ��С(�ֽ�)
//����ֵ:���䵽���ڴ��׵�ַ.
void *mymalloc(INT8U memx, INT32U size)  
{  
    INT32U offset;   
	//��ȡƫ�Ƶ�ַ
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


//���·����ڴ�(�ⲿ����)
//memx:�����ڴ��
//*ptr:���ڴ��׵�ַ
//size:Ҫ������ڴ��С(�ֽ�)
//����ֵ:�·��䵽���ڴ��׵�ַ.
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
	    mymemcpy((void*)((INT32U)malloc_dev.membase[memx] + offset), ptr, size);	//�������ڴ����ݵ����ڴ�   
        myfree(memx, ptr);  											  			//�ͷž��ڴ�
        return (void*)((INT32U)malloc_dev.membase[memx] + offset);  				//�������ڴ��׵�ַ
    }  
}



