#include "serial_config.h"
#include <string.h>
#include <stdlib.h>

void SERIAL_init()
{
	//---RCC config

	//Enable GPIO clock
	RCC_APB2PeriphClockCmd(USARTy_GPIO_CLK | USARTz_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

	#ifndef USE_STM3210C_EVAL
	//Enable USARTy Clock
	RCC_APB2PeriphClockCmd(USARTy_CLK, ENABLE);
	#else
	//Enable USARTy Clock
	RCC_APB1PeriphClockCmd(USARTy_CLK, ENABLE);
	#endif
	//Enable USARTz Clock
	RCC_APB1PeriphClockCmd(USARTz_CLK, ENABLE);
	
	//---GPIO config
	GPIO_InitTypeDef GPIO_InitStructure;

	#ifdef USE_STM3210C_EVAL
	//Enable the USART3 Pins Software Remapping
	GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);

	//Enable the USART2 Pins Software Remapping
	GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);  
	#elif defined USE_STM3210B_EVAL
	//Enable the USART2 Pins Software Remapping
	GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
	#endif

	//Configure USARTy Rx as input floating
	GPIO_InitStructure.GPIO_Pin = USARTy_RxPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USARTy_GPIO, &GPIO_InitStructure);

	//Configure USARTz Rx as input floating
	GPIO_InitStructure.GPIO_Pin = USARTz_RxPin;
	GPIO_Init(USARTz_GPIO, &GPIO_InitStructure);

	//Configure USARTy Tx as alternate function push-pull
	GPIO_InitStructure.GPIO_Pin = USARTy_TxPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(USARTy_GPIO, &GPIO_InitStructure);

	//Configure USARTz Tx as alternate function push-pull
	GPIO_InitStructure.GPIO_Pin = USARTz_TxPin;
	GPIO_Init(USARTz_GPIO, &GPIO_InitStructure);
	
	//---USARTy and USARTz configuration
	
	USART_InitTypeDef USART_InitStructure;
	/* USARTy and USARTz configured as follow:
	- BaudRate = 230400 baud  
	- Word Length = 8 Bits
	- One Stop Bit
	- Even parity
	- Hardware flow control disabled (RTS and CTS signals)
	- Receive and transmit enabled
	 */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	//Configure
	USART_Init(USARTy, &USART_InitStructure);
	USART_Init(USARTz, &USART_InitStructure);
	//Enable
	USART_Cmd(USARTy, ENABLE);
	USART_Cmd(USARTz, ENABLE);
}

void SERIAL_send_str(u8* buffer, u32 length)
{
	for( u32 i=0; i<length; i++)
	{
		USART_SendData(USARTy, buffer[i]);
		while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET);
	}
}

void SERIAL_send(u8* buffer)
{
	//the first 32 bit as size
	u32 len=0;
	for( len=0; buffer[len]!='\0' && len<1000000; len++) ;
	u8 head[4];
	*((u32*)head)=len;
	SERIAL_send_str(head,4);
	SERIAL_send_str(buffer,len);
}

void SERIAL_receive_str(u8* buffer, u32 length)
{
	u32 i;
	for( i=0; i<length; i++)
	{
		while(USART_GetFlagStatus(USARTy, USART_FLAG_RXNE) == RESET);
		buffer[i] = (USART_ReceiveData(USARTy) & 0xFF);
	}
	buffer[i] = '\0';
}

u8* SERIAL_receive()
{
	u8 head[5];
	SERIAL_receive_str(head,4);
	u32 len = *((u32*)head);
	if( !len) return 0;
	u8* buffer = (u8*)malloc(sizeof(u8)*(len+10));
	if(!buffer) {
		printf("malloc failed for %d bytes\n",sizeof(u8)*(len+10));
		printf("  at %s:%d\n",__FILE__,__LINE__);
		return 0;
	}
	SERIAL_receive_str(buffer,len);
	return buffer;
}

void SERIAL_freebuffer(u8* buf)
{
	if( buf)
		free(buf);
}

bool SERIAL_selftest()
{
	u8 buffer[] = "This is a self test of serial transfer from USARTy to USARTz";
	u8 rxbuffer[sizeof(buffer)/sizeof(*(buffer))];
	u32 len = sizeof(buffer)/sizeof(*(buffer));
	// warm up
	USART_SendData(USARTy, 'T');
	while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET);
	while(USART_GetFlagStatus(USARTz, USART_FLAG_RXNE) == RESET);
	USART_ReceiveData(USARTz);
	// transfer
	for( u32 i=0; i<len; i++)
	{
		//Send one byte from USARTy
		USART_SendData(USARTy, buffer[i]);
		//wait until USARTy DR register is empty
		while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET);
		//until the USARTz Receive Data Register is not empty
		while(USART_GetFlagStatus(USARTz, USART_FLAG_RXNE) == RESET);
		//Store the received byte
		rxbuffer[i] = (USART_ReceiveData(USARTz) & 0x7F);
	}
	LOG(rxbuffer);
	bool success = strcmp((char*)buffer,(char*)rxbuffer)==0;
	return success;
}
