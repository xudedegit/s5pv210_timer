
#define GPD0_CON 		(0xE02000A0)
#define TCFG0			(0xE2500000)
#define TCFG1			(0xE2500004)
#define CON				(0xE2500008)
#define TCNTB2			(0xE2500024)
#define TCMPB2			(0xE2500028)


#define rGPD0_CON       (*(volatile unsigned int *)GPD0_CON)
#define rTCFG0       	(*(volatile unsigned int *)TCFG0)
#define rTCFG1       	(*(volatile unsigned int *)TCFG1)
#define rCON       		(*(volatile unsigned int *)CON)
#define rTCNTB2      	(*(volatile unsigned int *)TCNTB2)
#define rTCMPB2       	(*(volatile unsigned int *)TCMPB2)



//设置PWM波形为2KHZ
void pwm_init()
{
	//设置GPDO_CON的模式
	rGPD0_CON &= ~(0xf<<8);
	rGPD0_CON |= (2<<8);
	
	//设置与定时器相关的寄存器
	//将rTCFG0的8~15位设置成65，也就是将预分频器设置成65，最终分频为1/66
	//由于接入的时钟是PCLK_PSYS，为66.7MHZ，分频之后为1MHZ
	rTCFG0 &= ~(0xff<<8);
	rTCFG0 |= (65<<8);
	
	//设置分频器为2分频，设置rTCFG1的8~11位设置为0001，分频后为500KHZ
	rTCFG1 &= ~(0xf<<8);
	rTCFG1 |= (1<<8);
	
	
	rCON |= (1<<15); //打开自动重载功能
	
	//设置rTCNTB和rTCMPB寄存器
	rTCNTB2 = 625;   
	rTCMPB2 = 312.5;  //设置占空比为50%
	
	//设置CON寄存器
	
	
	
	rCON |= (1<<13);  //打开手工更新功能
	rCON &= ~(1<<13);
	
	rCON |= (1<<12);  //开启Timer2
	}
	



























