#include <stdio.h>
#include "serial.c"

void PROTO_init()
{
	SERIAL_init();
}

void PROTO_send(u8* buffer)
{
	SERIAL_send(buffer);
}

OPER* PROTO_receive()
{
	u8* buffer = SERIAL_receive();
	if( !buffer) return 0;
	OPER* inst = (OPER*)buffer;
	return inst;
}

void PROTO_log(const OPER* o)
{
	char buf[100];
	snprintf(buf,100,"[%c,%d,%d]",o->code,o->x,o->y);
	LOG((u8*)buf);
}

void PROTO_free(OPER* oper)
{
	if( oper)
		free(oper);
}
