#include <stdio.h>
#include "stm32f10x.h"

void LongDelay(u32 nCount);
#include "log.c"
#include "plotter.c"
#include "protocol.c"

void everyframe();
void every10frame();
void every100frame();

PLOTTER plotobject;
PLOTTER* plot = &plotobject;

int main(void)
{
	SystemInit();
	PWM_init();
	PLOTTER_init(plot);
	PLOTTER_update(plot);
		//PLOTTER_touchinit();
	LOG_init();
	PROTO_init();

	PROTO_send("hello from STM32");
	while (1)
	{
		OPER* inst = PROTO_receive();
		if( inst)
		{
			plot->oper = inst;
			plot->opercurr = -1;
			for( u32 N=0; ; N++)
			{
				if( PLOTTER_update(plot)=='Q')
					break;

					everyframe();
				if( N%10==0)
					every10frame();
				if( N%100==0)
					every100frame();
			}
			PROTO_free(inst);
		}
		else
		{
			PROTO_send("error: PROTO_receive() returns 0");
			continue;
		}
		PROTO_send("operations finished");
	}
}

void everyframe()
{
}

void every10frame()
{
}

void every100frame()
{
	/*
	if(GPIOB->IDR & 0x00000800)
	{

		GPIOF->BRR = GPIO_Pin_6;
		plot->penisdown = 1;
	}
	else

	{
		GPIOF->BSRR = GPIO_Pin_6;
		plot->penisdown = 0;
	} */
}

void LongDelay(u32 N)
{
	for(; N != 0; N--)
	{
			everyframe();
		if( N%10==0)
			every10frame();
		if( N%100==0)
			every100frame();
	}
}                                                                                        
