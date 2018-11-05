
/* Modu�:      RTC (Real Time Clock)
 *             Zegar czasu rzeczywistego (z rozszerzeniem o obs�ug� program�w)
 * Autor:      Miros�aw Szajner
 * Klasa:      V TEL
 * Rok:        2002/2003
 * Copyright:  Miros�aw Szajner
 *
 * Modu� wykorzystuje Timer 2 do obs�ugi czasu rzeczywistego, Timer 1 jako licznik
 * przy odbiorze sygna�u DCF, przerwanie INT0 do odbioru impuls�w DCF.
 */

#ifndef RTC_H
#define RTC_H

#include "global.h"

extern volatile TTime     Time;          // Czas wyeksportowany z modu�u (do u�yt. zewn.)
extern volatile TProgram  Program;       // Program wyeksportowany z modu�u (do u�yt. zewn.)

extern void RtcInit    (void);           // Funkcja inicjalizuje modu�
extern void ProgramLoad(void);           // Prze�adowanie programu
extern void DcfEnable  (void);           // W��czenie odbiornika DCF
extern void DcfDisable (void);           // Wy��czenie odbiornika DCF

#endif
