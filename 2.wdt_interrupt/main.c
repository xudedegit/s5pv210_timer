#include "stdio.h"
#include "wt.h"
#include "Interrupt.h"



int main()
{
	
	uart_init();
	wdt_init_interrupt();
	
	//绑定异常向量表
	system_init_exception();
	
	
	
	printf("-------wdt interrupt test----------");
	
	//绑定isr到中断控制器硬件上
	intc_setvectaddr(NUM_WDT,isr_wdt);
	
	//使能中断
	intc_enable(NUM_WDT);
	
	while(1);
	
	return 0;
}




























