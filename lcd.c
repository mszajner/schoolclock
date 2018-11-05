
#include <io.h>
#include <stdarg.h>
#include <pgmspace.h>
#include "lcd.h"

void LcdDelay(unsigned int d)
{
	unsigned char t;
	while(d--)
		for (t=0; t<255; t++);
}

void Lcd4PutC(unsigned char byte)
{
	LCD_PORT = (byte & 0xF0) | BV(LCD_PIN_ENABLE) | (LCD_PORT & (BV(LCD_PIN_LIGHT) | BV(LCD_PIN_RS)));
	LcdDelay(LCD_DELAY_100us);
	cbi(LCD_PORT, LCD_PIN_ENABLE);
}

void Lcd8PutC(unsigned char byte)
{
	Lcd4PutC(byte);
	Lcd4PutC(byte<<4);
}

void LcdChar(unsigned char c)
{
	sbi(LCD_PORT, LCD_PIN_RS); 
	Lcd8PutC(c);          
}

void LcdControl(unsigned char c)
{
	cbi(LCD_PORT, LCD_PIN_RS); 
	Lcd8PutC(c);          
}

#ifndef LcdGotoxy
void LcdGotoxy(unsigned char x, unsigned char y)
{
	LcdControl(0x80 + y*0x40 + x);
	LcdDelay(LCD_DELAY_100us);
}
#endif

void LcdInit(void)
{
	LcdDelay(LCD_DELAY_16ms);      // wait for internal reset circuit in LCD
	LCD_DIRECTION = 0xFF;          // all pins output
	cbi(LCD_PORT, LCD_PIN_RS);     // writing to instruction register
	Lcd4PutC(0x30);                // send command=0x30, select bus type=8 pin
	LcdDelay(LCD_DELAY_4ms);       // wait for execute previous command
	Lcd4PutC(0x30);                // once more, send comand=0x30
	LcdDelay(LCD_DELAY_1ms);       // wait for execute that command
	Lcd4PutC(0x30);                // and last select bus type=8pin
	Lcd4PutC(0x20);                // select bus type=4pin
	Lcd8PutC(0x28);                // once more select bus type=4pin, lines=2
	Lcd8PutC(0x0C);                // display on, curosr off, blinking off
	Lcd8PutC(0x06);                // entry mode set=increment writing
	Lcd8PutC(0x01);                // clear display
}

void LcdChars(unsigned char c, unsigned char n)
{
	while(n--)
		LcdChar(c);
}

void LcdPrintf(unsigned char *Format, ...)
{
	static unsigned char Hex[] PROGMEM = { "0123456789ABCDEF" };
	unsigned char FormatFlag;
	unsigned char *Ptr;
	prog_char *PrgCh;
	unsigned int Uval, Dval, Base;
	va_list Ap;
	va_start (Ap, Format);
	while(1)
	{
   	while ((FormatFlag = PRG_RDB(Format++)) != '%')      // Until '%' or '\0'
   	{
      	if (!FormatFlag)
      	{
      		va_end (Ap);
         	return;
      	}	
      	LcdChar(FormatFlag);
      }
		switch (FormatFlag = PRG_RDB(Format++))
		{
      	case 'c':
      		FormatFlag = va_arg(Ap, int);
      	default:
      		LcdChar(FormatFlag);
      		continue;
      	case 'n':
      		LcdLowerline();
      		continue;
      	case '~':
      		LcdClear();
      		continue;
      	case 's':
      		PrgCh = (prog_char*)va_arg(Ap, prog_char*);
      		while(*PrgCh)
      			LcdChar(*PrgCh++);
      		continue;
      	case 'f': // string in flash
      		Ptr = (unsigned char*)va_arg(Ap, unsigned char*);
      		while(PRG_RDB(Ptr))
      			LcdChar(PRG_RDB(Ptr++));
      		continue;
      	case '*': // max delay
      		LcdDelay(LCD_DELAY_MAX);
      		continue;
      	case 'l': //lowerline
      	   LcdLowerline();
      	   continue;
      	case 'u': // upperline
      	   LcdUpperline();
      	   continue;
      	case 'b': // a'la BCD
      		Base = 10;
      		Dval = 10;
      		goto CONVERSION_LOOP;
      	case 'd':
      		Base = 10;
      		Dval = 10000;
      		goto CONVERSION_LOOP;
      	case 'x':
      		Base = 16;
      		Dval = 0x10;
      	CONVERSION_LOOP:
       		Uval = va_arg(Ap, int);
       		if (FormatFlag == 'b')
       			goto BCD_LOOP;
       		if (FormatFlag == 'd')
       		{
         		if (((int)Uval) < 0)
         		{
           			Uval = - Uval;
           			LcdChar('-');
         		}
         		while (Dval > 1 && Dval > Uval)
         			Dval /= 10;
        		}
        	BCD_LOOP:
       		do {
         		LcdChar(PRG_RDB(Hex + (Uval / Dval)));
         		Uval %= Dval;
         		Dval /= Base;
       		} while (Dval);
    	}
  }
}
