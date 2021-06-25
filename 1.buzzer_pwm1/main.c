#include <stdio.h>

#include "pwm.h"

void uart_init(void);

void delay()
{
	int i=10000;
	while(i--);
	
}

int main()
{

	uart_init();
	
	pwm_init();
	
	while(1)
	{
		putc('a');
		delay();
	}
	return 0;

}
