
#include "wt.h"

void intc_clearvectoraddr();

void wdt_init_reset(void)
{
	//设置WTCON中的预分频器部分
	
	rWTCON &= ~(0xff<<8);
	rWTCON |= (65<<8);       //预分频器的分频系数设置成65，则实际预分频器为1/66分频
	
	rWTCON &= ~(3<<3);
	rWTCON |= (3<<3);      //分频器设置为128分频
	
	rWTCON &= ~(1<<2);     //关闭中断生成器
	rWTCON |= (1<<0);     //打开复位功能
	
	
	
	rWTDAT = 10000;
	rWTCNT = 10000;     //因为看门狗寄存器没有手动刷新位的设置，所以需要通过赋初值为WTCNT寄存器赋值，否则将按照默认值开始工作
	
	rWTCON |= (1<<5);   //开看门狗
	
	
}

void isr_wdt(void)
{
	static int i =1;
	printf("isr_wdt reset i=%d......",i++);
	
	//关中断
	intc_clearvectoraddr();
	rWTCLRINT = 1;
	
}



















