#include "stdio.h"
#include "wt.h"
#include "Interrupt.h"



int main()
{
	
	static unsigned int i =1;
	
	uart_init();
	wdt_init_reset();
	

	
	printf("-------wdt reset test i=%d----------",i++);
	
	while(1);
	
	return 0;
}




























