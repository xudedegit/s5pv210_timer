#include "stdio.h"
#include "Interrupt.h"



//中断处理的步骤分为两个阶段
//第一阶段是绑定异常向量表

//对中断控制器的基本寄存器进行初始化，初始化步骤包括：
//1.禁止所有中断
//2.选择中断类型为IRQ
//3.清除VICxADDR寄存器

//第二阶段是查找中断源；找到isr






//绑定异常向量表，若产生哪种异常时，硬件会自动跳转到异常向量（异常处理程序）
/*
void LinkExceptionVectorTable()
{
	rReset_Exception 	 = (unsigned int)Reset_ExceptionFunc;
	rUndefined_Exception = (unsigned int)Undefined_ExceptionFunc;
	rSoft_Exception		 = (unsigned int)Soft_ExceptionFunc;
	rPrefetch_Exception  = (unsigned int)Prefetch_ExceptionFunc;
	rData_Exception		 = (unsigned int)Data_ExceptionFunc;
	rIRQ_Exception		 = (unsigned int)IRQ_handle;
	rFIQ_Exception		 = (unsigned int)IRQ_handle;
	
	//初始化中断控制器的基本寄存器
	intc_init();
}
*/
void system_init_exception(void)
{
	// 第一阶段处理，绑定异常向量表
	rReset_Exception = (unsigned int)Reset_ExceptionFunc;
	rUndefined_Exception = (unsigned int)Undefined_ExceptionFunc;
	rSoft_Exception = (unsigned int)Soft_ExceptionFunc;
	rPrefetch_Exception = (unsigned int)Prefetch_ExceptionFunc;
	rData_Exception = (unsigned int)Data_ExceptionFunc;
	rIRQ_Exception = (unsigned int)IRQ_handle;
	rFIQ_Exception = (unsigned int)IRQ_handle;
	
	// 初始化中断控制器的基本寄存器
	intc_init();
}

//清除isr地址寄存器，该寄存器用来存放isr地址(中断处理程序的地址)，
//该地址是由硬件自动推入的，所以在初始化阶段先将其内部清空。
/*
void intc_clearvectoraddr(void)
{
	
	VIC0ADDRESS = 0;
	VIC1ADDRESS = 0;
	VIC2ADDRESS = 0;
	VIC3ADDRESS = 0;
}
*/
void intc_clearvectoraddr(void)
{
    // VICxADDR:当前正在处理的中断的中断处理函数的地址
	VIC0ADDRESS = 0x0;
    VIC1ADDRESS = 0x0;
    VIC2ADDRESS = 0x0;
    VIC3ADDRESS = 0x0;
}
void intc_init(void)
{
	//禁止所有中断
	VIC0INTENCLEAR=0xFFFFFFFF;
	VIC1INTENCLEAR=0xFFFFFFFF;
	VIC2INTENCLEAR=0xFFFFFFFF;
	VIC3INTENCLEAR=0xFFFFFFFF;
	
	
	//选择中断类型为IRQ
	VIC0INTSELECT=0x0;
	VIC1INTSELECT=0x0;
	VIC2INTSELECT=0x0;
	VIC3INTSELECT=0x0;
	
	
	//清VICxADDR
	intc_clearvectoraddr();
}

void intc_setvectaddr(unsigned long interruptNum,void (*handle)(void))
{
	//VIC0
	if(interruptNum<32)
	{
		*((volatile unsigned long*)(VIC0VECTADDR + 4*(interruptNum - 0)))=(unsigned)handle;
	}
	else if(interruptNum<64)
	{
		*((volatile unsigned long*)(VIC1VECTADDR + 4*(interruptNum - 32)))=(unsigned)handle;
	}
	else if(interruptNum<96)
	{
		*((volatile unsigned long*)(VIC2VECTADDR + 4*(interruptNum - 64)))=(unsigned)handle;
	}
	else
	{
		*((volatile unsigned long*)(VIC3VECTADDR + 4*(interruptNum - 96)))=(unsigned)handle;
	}
	
	return;
	
}

void intc_enable(unsigned long interruptNum)
{
	if(interruptNum<32)
	{
		VIC0INTENABLE |= (1<<(interruptNum - 0));
	}
	else if(interruptNum<64)
	{
		VIC1INTENABLE |= (1<<(interruptNum - 32));
	}
	else if(interruptNum<96)
	{
		VIC2INTENABLE |= (1<<(interruptNum - 64));
	}
	else if(interruptNum<200)
	{
		VIC3INTENABLE |= (1<<(interruptNum - 96));
	}
	
	else
	{
		VIC0INTENABLE = 0xFFFFFFFF;
		VIC1INTENABLE = 0xFFFFFFFF;
		VIC2INTENABLE = 0xFFFFFFFF;
		VIC3INTENABLE = 0xFFFFFFFF;
	}
	
}

void intc_disable(unsigned long interruptNum)
{
	if(interruptNum<32)
	{
		VIC0INTENCLEAR |= (1<<(interruptNum - 0));
	}
	else if(interruptNum<64)
	{
		VIC1INTENCLEAR |= (1<<(interruptNum - 32));
	}
	else if(interruptNum<96)
	{
		VIC2INTENCLEAR |= (1<<(interruptNum - 64));
	}
	else if(interruptNum<200)
	{
		VIC3INTENCLEAR |= (1<<(interruptNum - 96));
	}
	else
	{
		VIC0INTENCLEAR = 0xFFFFFFFF;
		VIC1INTENCLEAR = 0xFFFFFFFF;
		VIC2INTENCLEAR = 0xFFFFFFFF;
		VIC3INTENCLEAR = 0xFFFFFFFF;
		
	}
}


void Reset_ExceptionFunc(void)
{
	printf("Reset_Exception handle.\n");
	
}

void Undefined_ExceptionFunc(void)
{
	
	printf("Undefined_Exception handle.\n");
}

void Soft_ExceptionFunc(void)
{
	
	printf("Soft_Exception handle.\n");
}


void Prefetch_ExceptionFunc(void)
{
	printf("Prefetch_Exception handle.\n");
}

void Data_ExceptionFunc(void)
{
	
	printf("Data_Exception handle.\n");
}

unsigned long IRQStatus(int interruptNum)
{
	switch(interruptNum)
	{
		case 1:
			return VIC0IRQSTATUS;
		case 2:
			return VIC1IRQSTATUS;
		case 3:
			return VIC2IRQSTATUS;
		case 4:
			return VIC3IRQSTATUS;
		default:
			return -1;
		
	}
	
}

//真正处理中断程序的函数，不包括现场保护和恢复现场
void irq_handler(void)
{
	unsigned long addr[4]={VIC0ADDRESS,VIC1ADDRESS,VIC2ADDRESS,VIC3ADDRESS};
	
	void (*isr)(void)=NULL;
	
	int i=0;
	for (i=0;i<4;i++)
	{
		if(IRQStatus(i)!=0)
		{
			isr= (void (*)(void)) addr[i];
			break;
		}
	}
	
	isr();   //调用真正的isr程序
	
}

































