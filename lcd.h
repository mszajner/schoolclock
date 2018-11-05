
/* Modu³:      LCD
 *             Wyœwietlacz LCD
 * Autor:      Miros³aw Szajner
 * Klasa:      V TEL
 * Rok:        2002/2003
 * Copyright:  Miros³aw Szajner
 *
 */

#ifndef LCD_H
#define LCD_H

#include "global.h"

extern void LcdInit(void);                              // Inicjalizuje wyswietlacz
extern void LcdPrintf(unsigned char *format, ...);      // Wyswietla sformatowany tekst
extern void LcdChar(unsigned char c);                   // Wyswietla jeden znak
extern void LcdControl(unsigned char c);                // Wysyla polecenie
extern void LcdChars(unsigned char c, unsigned char n); // Wyswietla ciag takich samych znakow
extern void LcdDelay(unsigned int d);                   // Generuje opoznienie
//extern void LcdGotoxy(unsigned char x, unsigned char y);

#define LcdClear()                          LcdControl(0x01)
#define LcdHome()                           LcdControl(0x02)
#define LcdMode(increment, display)         LcdControl(0x04 | ((increment)<<1) | ((display)<<1))
#define LcdDisplay(display, cursor, blink)  LcdControl(0x08 | ((display)<<2) | ((cursor)<<1) | (blink))
#define LcdShift(display, left)             LcdControl(0x10 | ((display)<<3) | ((left)<<2))
#define LcdFunctionSet(bus, lines, font)    LcdControl(0x20 | ((bus)<<4) | ((lines)<<3) | ((font)<<2)
#define LcdSetCGRAMAddress(address)         LcdControl(0x40 | (address))
#define LcdGotoxy(x, y)                     LcdControl(0x80 | ((y)<<6) | (x))
#define LcdUpperline()                      LcdGotoxy(0, 0)
#define LcdLowerline()                      LcdGotoxy(0, 1)

#define LcdLightOn()                        sbi(LCD_PORT, LCD_PIN_LIGHT)
#define LcdLightOff()                       cbi(LCD_PORT, LCD_PIN_LIGHT)

#endif
