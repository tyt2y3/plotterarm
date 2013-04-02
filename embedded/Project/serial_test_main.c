#include "stm32f10x.h"
#include "log.c"
#include "serial.c"

void LongDelay(u32 N);

int main(void)
{
	SystemInit();

	//init
	LOG_init();
	SERIAL_init();

	//SERIAL_selftest();
	while (1)
	{
		SERIAL_send("hello from STM32");
		u8* buffer = SERIAL_receive();
		LOG(buffer);
		SERIAL_freebuffer(buffer);
	}
}

void LongDelay(u32 N)
{
	for(; N != 0; N--)
	{
	}
}