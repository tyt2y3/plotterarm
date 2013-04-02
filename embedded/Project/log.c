#include "lcd.h"
//requires lcd.c in linking

u8 LOG_counterx = 0; //horizontal cursor
u8 LOG_countery = 0;
u8 LOG_width = 128; //width of LCD
u8 LOG_height = 8;
u8 LOG_charw = 8; //width of a character
u8 LOG_charh = 2;

void LOG_str(const u8* string, u16 length)
{
	for( u8 i=0; i<length+1; i++)
	{
		if( i==length)
			LCD_DrawChar(LOG_countery,LOG_counterx,'_');
		else
			LCD_DrawChar(LOG_countery,LOG_counterx,string[i]);
		LOG_counterx += LOG_charw;
		if( LOG_counterx >= LOG_width)
		{
			LOG_counterx = 0;
			LOG_countery += LOG_charh;
		}
		if( LOG_countery >= LOG_height)
		{
			LOG_countery = 0;
		}
	}
}

void LOG(const u8* string)
{
	if( string)
	{
		u16 i;
		for( i=0; string[i]!='\0' && i<100; i++) ;
		LOG_str( string, i);
	}
	else
		LOG_str( "null", 4);
}

void LOG_init()
{
	// LCD config
	// Enable FSMC, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE |
							RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG, ENABLE);
	STM3210E_LCD_Init();
	LCD_Clear();
}
