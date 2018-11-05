
/* Modu³:      RTC (Real Time Clock)
 *             Zegar czasu rzeczywistego (z rozszerzeniem o obs³ugê programów)
 * Autor:      Miros³aw Szajner
 * Klasa:      V TEL
 * Rok:        2002/2003
 * Copyright:  Miros³aw Szajner
 *
 * Modu³ wykorzystuje Timer 2 do obs³ugi czasu rzeczywistego, Timer 1 jako licznik
 * przy odbiorze sygna³u DCF, przerwanie INT0 do odbioru impulsów DCF.
 */

#ifndef RTC_H
#define RTC_H

#include "global.h"

extern volatile TTime     Time;          // Czas wyeksportowany z modu³u (do u¿yt. zewn.)
extern volatile TProgram  Program;       // Program wyeksportowany z modu³u (do u¿yt. zewn.)

extern void RtcInit    (void);           // Funkcja inicjalizuje modu³
extern void ProgramLoad(void);           // Prze³adowanie programu
extern void DcfEnable  (void);           // W³¹czenie odbiornika DCF
extern void DcfDisable (void);           // Wy³¹czenie odbiornika DCF

#endif
