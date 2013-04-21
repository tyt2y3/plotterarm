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

OPER* PROTO_deserialize(const u8* buffer)
{
	u16 length=1;
	OPER Oper;
	OPER* oper=&Oper;
	for( u16 i=0; i<length;)
	{
		oper[i].code = buffer[i*5];
		oper[i].x = *((s16*)&buffer[i*5+1]);
		oper[i].y = *((s16*)&buffer[i*5+3]);
		if( i==0)
		{
			if( oper[i].code=='^' && (u8)oper[i].x=='H')
			{
				length = (u16)oper[i].y;
				i=0;
				oper = (OPER*)malloc(length*sizeof(OPER));
				if( !oper) return 0;
				buffer+=5;
				continue;
			}
			else if ( oper==&Oper)
				return 0;
		}
		i++;
	}
	return oper;
}

OPER* PROTO_receive()
{
	u8* buffer = SERIAL_receive();
	if( !buffer) return 0;
	OPER* inst = PROTO_deserialize(buffer);
	if( !inst) return 0;
	SERIAL_freebuffer(buffer);
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
