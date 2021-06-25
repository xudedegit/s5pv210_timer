#ifndef __WT_H__
#define __WT_H__


#define WTCON 		(0xE2700000)
#define WTDAT 		(0xE2700004)
#define WTCNT 		(0xE2700008)
#define WTCLRINT	(0xE270000C)

#define rWTCON		(*(volatile unsigned int *)WTCON)
#define rWTDAT		(*(volatile unsigned int *)WTDAT)
#define rWTCNT		(*(volatile unsigned int *)WTCNT)
#define rWTCLRINT	(*(volatile unsigned int *)WTCLRINT)

void wdt_init_reset();
void isr_wdt();

#endif
















