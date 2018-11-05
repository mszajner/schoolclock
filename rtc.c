
/* Modu³:      RTC (Real Time Clock) + EXT (Extensions)
 *             Zegar czasu rzeczywistego (z rozszerzeniem o obs³ugê programów)
 * Autor:      Miros³aw Szajner
 * Klasa:      V TEL
 * Rok:        2002/2003
 * Copyright:  Miros³aw Szajner
 *
 */

#include <io.h>
#include <sig-avr.h>
#include <string-avr.h>
#include <eeprom.h>
#include "rtc.h"

#define START_TIMER1     outp(0x03, TCCR1B)                 // Uruchamianie Timera 1
#define STOP_TIMER1      outp(0x00, TCCR1B)                 // Zatrzymywanie Timera 1
#define START_TIMER2     outp(0x05, TCCR2)                  // Uruchamianie Timera 2
#define STOP_TIMER2      outp(0x00, TCCR2)                  // Zatrzymywanie Timera 2
#define ASYNC_TIMER2     outp(BV(AS2), ASSR)                // Ustawianie asynch. obs³ugi Timera 2
#define UPDATING_TIMER2  (ASSR & 0x07)                      // Sprawdzanie konf. Timera 2 zmieniona
#define START_INT0       outp(BV(INT0), GIMSK)              // W³¹czenie przerwania zewn. INT0
#define STOP_INT0        outp(0x00, GIMSK)                  // Wy³¹czenie przerwania zewn. INT0
#define FALLING_INT0     outp(0x02, MCUCR)                  // INT0 wywo³ywanie zboczem opadaj¹cym
#define RISING_INT0      outp(0x03, MCUCR)                  // INT0 wywo³ywanie zboczem narastaj¹cym
#define ENABLE_TIMERS    outp(BV(TOIE1) | BV(TOIE2), TIMSK) // W³¹czenie Timera 1 i 2

	volatile TTime      Time;                           // Czas zegara RTC
	volatile TTime      Correct;                        // Czas odberany z DCF
	volatile TProgram   Program;                        // Program
	volatile char       CorrectTime;                    // Poprawianie czasu letniego/zimowego

/* RtcInit: Inicjalizacja modu³u
 */
void RtcInit(void)
{
	ASYNC_TIMER2;            // Ustaw Timer 2 w pracê asynchroniczn¹
	START_TIMER2;            // Uruchom Timer 2
	while(UPDATING_TIMER2);  // Poczekaj na aktualizacjê ustawieñ Timera 2
	FALLING_INT0;            // Ustaw INT0 wywo³ywane zboczem opadaj¹cym
	ENABLE_TIMERS;           // W³¹cz Timer 1 i 2
	DcfInit();               // Skonfiguruj port/pin odbiornika DCF
	RingInit();              // Skonfiguruj port/pin dzwonka
	CorrectTime = 0;         // Nie zmieniono jeszcze czasu letniego/zimowego
}

/* DcfEnable: W³¹czanie odbiornika DCF
 */
void DcfEnable(void)
{
	DcfOn();                // W³¹cz odbiornik DCF
	START_INT0;              // W³¹cz przerwanie INT0
	START_TIMER1;            // W³¹cz Timer 1
}

/* DcfDisable: Wy³¹czanie odbiornika DCF
 */
void DcfDisable(void)
{
	STOP_TIMER1;             // Wy³¹cz Timer 1
	STOP_INT0;               // Wy³¹cz przerwanie INT0
	DcfOff();               // Wy³¹cz odbiornik DCF
}

/* CompareTimes: Porównywanie dwóch czasów
 * Zwraca:
 *   -1, jeœli czas (hour1:minute1) < (hour2:minute2)
 *    0, jeœli czasy s¹ równe
 *    1, jeœli czas (hour1:minute1) > (hour2:minute2)
 */
char CompareTimes(unsigned char hour1, unsigned char minute1, // czas (hour1:minute1)
                   unsigned char hour2, unsigned char minute2) // czas (hour2:minute2)
{
	if (hour1==hour2)                                  // jeœli godziny s¹ równe
	{
		if (minute1==minute2)                      // i jeœli minuty s¹ równe
			return 0;                          // zwróæ O (zero)
		else                                       // w przeciwnym wypadku
			return minute1>minute2 ? 1 : -1;   // zwróæ 1, jeœli minute1>minute2, albo -1
	}
	else                                               // jeœli godziny s¹ ró¿ne
		return hour1>hour2 ? 1 : -1;               // zwróæ 1, jeœli hour1>hour2, albo -1
}

/* FindPrg:  Przeszukiwanie programu w celu ustalenia
 *           aktualnie trwaj¹cej pozycji, i czasu
 *           uruchomienia nastêpnej
 */
void FindPrg(void)
{
	unsigned char Hour, Minute;                                // zmienne pomocnicze
	unsigned int Address = 0x100 + 0x40*Program.Number;        // adres pocz¹tku programu w EEPROM
	Program.Minute = -1;
	Program.Count = eeprom_rb(Address++)<<1;                   // odczytaj iloœæ pozycji
	if (Program.Calendar & BV(Time.DayOfWeek))                 // Jeœli dzieñ aktywny,
	for (Program.Index=-1; Program.Index < Program.Count-1; Program.Index++)  // pêtla przeszukuj¹ca
	{
		Hour = eeprom_rb(Address++);                       // odczytaj godzinê
		Minute = eeprom_rb(Address++);                     // odczytaj minutê
		if (CompareTimes(Hour, Minute, Time.Hour, Time.Minute) > 0)
		{
			Program.Hour = Hour;
			Program.Minute = Minute;
			return;
		}
	}
	Program.Index = -1; // jeœli nie znaleziono ¿aden pozycji tzn. ¿e program zakoñczono
}

/* ProgramLoad: Prze³adowywanie programu, czyli
 *             ponowny odczyt programu z pamiêci
 */
void ProgramLoad(void)
{
	eeprom_read_block((char*)&Program, EEPROM_PROGRAM_NUMBER, 3);
   FindPrg();                                            // znajdŸ aktualnie wykonywan¹ pozycjê
}

/* not_leap: Funkcja sprawdzaj¹ca czy aktualnie trwa rok przestêpny
 * Zwraca:
 *   nie zero, gdy prawda
 */
inline char NotLeap(void)
{
   if (!(Time.Year%100))              // jeœli rok podzielny przez 100
      return (char)(Time.Year%400);   // to zwróæ niezero, gdy podzielny te¿ przez 400 (Y2k)
   else
      return (char)(Time.Year%4);     // inaczej zwróæ niezero, gdy rok podzielny przez 4
}

/* INTERRUPT FROM SIGNAL OVERFLOW 2:
 * Przerwanie obs³ugi Timera 2 (RTC)
 */
INTERRUPT(SIG_OVERFLOW2)
{
   /* podstawowa obs³uga zegara RTC */
   Time.Changed++;                            // zwiêksz aby poinformowaæ o zmianie czasu
   if (++Time.Second >= 60)                   // zwiêksz sekundy, i gdy równe 60
   {
      Time.Second = 0;                        // wyzeruj sekundy
      if (++Time.Minute >= 60)                // zwiêksz minuty, i gdy równe 60
      {
         Time.Minute = 0;                     // wyzeruj minuty
         if (++Time.Hour >= 24)               // zwiêksz godziny, i gdy równe 24
         {
            Time.Hour = 0;                    // wyzeruj godziny
            if (++Time.DayOfWeek >= 7)        // zwiêksz dni tygodnia, i gdy równe 7
               Time.DayOfWeek = 0;            // wyzeruj dni tygodnia
            if (++Time.Day >= 32)             // zwiêksz dni miesi¹ca, i gdy równe 32
            {
               Time.Month++;                  // zwiêksz miesi¹ce
               Time.Day = 1;                  // ustaw pierwszy dzieñ miesi¹ca
               CorrectTime = 0;               // wyzeruj znacznik zmiany czasu letniego/zimowego
            }
            else if (Time.Day == 31)          // inaczej, jeœli dni miesi¹ca równe 31
            {                                 // i aktualny miesi¹c, to
               if ((Time.Month==4) ||         // kwiecieñ lub
                   (Time.Month==6) ||         // czerwiec lub
                   (Time.Month==9) ||         // wrzesieñ lub
                   (Time.Month==11))          // listopad
               {
                  Time.Month++;               // zwiêksz miesi¹ce
                  Time.Day = 1;               // ustaw pierwszy dzieñ miesi¹ca
               }
            }
            else if (Time.Day == 30)          // inaczej, jeœli dni miesi¹ca równe 30
            {
               if (Time.Month == 2)           // jeœli aktualny miesi¹c to luty, to
               {
                  Time.Month++;               // zwiêksz miesi¹ce
                  Time.Day = 1;               // ustaw pierwszy dzieñ miesi¹ca
               }
            }
            else if (Time.Day == 29)          // inaczje, jeœli dni miesi¹ca równe 29
            {
               if ((Time.Month == 2) &&       // i miesi¹c luty
                   (NotLeap()))               // oraz rok nie przestêpny
               {
                  Time.Month++;               // to zwiêksz miesi¹ce
                  Time.Day = 1;               // ustaw pierwszy dzieñ miesi¹ca
               }
            }
            if (Time.Month >= 13)             // jeœli miesi¹c równy 13
            {
               Time.Month = 1;                // ustaw miesi¹c styczeñ
               Time.Year++;                   // zwiêksz rok
            }
         }
      }
   }
   /* obsluga czasu letniego/zimowego */
   if (Time.DayOfWeek == 6 &&                 // jeœli dzieñ tygodnia=niedziela i dzieñ miesi¹ce>25
       Time.Day > 25 && !CorrectTime)         // oraz jeszcze nie poprawiano czasu w tym miesi¹cu, to
   {
      if (Time.Month == 3 && Time.Hour == 2)  // jeœli marzec i godzina=2
         Time.Hour++;                         // zwiêksz godzinê
      if (Time.Month == 10 && Time.Hour == 3) // jeœli pazdziernik i godzina=4 oraz jeszcze nie 
         Time.Hour--;                         // zmniejsz godzinê
      CorrectTime++;
   }
   /* obs³uga odbiornika DCF */
   if (Time.Hour == 4 && !Time.Minute &&      // jeœli godzina=4 i dzieñ tygodnia=niedziela
      !Time.Second && Time.DayOfWeek == 6)
      DcfEnable();                            // w³¹cz odbiornik DCF
   /* obs³uga programów */
   if (!Time.Hour && !Time.Minute && !Time.Second)  // jeœli godzina zerowa, prze³aduj program
      ProgramLoad();                                // na nowy dzieñ
   {
      if ((Time.Hour == Program.Hour) &&            // jeœli aktualna godzina = godzinie nastêpnego dzwonka
          (Time.Minute == Program.Minute))          // oraz minuta = minucie nastêpnego dzwonka
      {
         if (Time.Second == 0 && Program.Enabled)   // i sekunda = 0 oraz program w³¹czony
            RingOn();                               // w³¹cz dzwonek
         if (Time.Second == 5)                      // jeœli sekunda = 5, to
         {
            RingOff();                              // wy³¹cz dzownek
            FindPrg();                              // znajdŸ nastêpny czas dzwonienia
         }
      }
   }
}

/* INTERRUPT FROM SIGNAL INT 0:
 * Przerwanie zewnêtrzne INT0
 * Przerwanie odbioru danych DCF
 */
INTERRUPT(SIG_INTERRUPT0)
{
	flag = flag ? 0 : 1;
	if (flag)
	{
		INT_RISING;             // nastepne zbocze wzrastajace
		CLEART_TIMER1;          // wyzerowanie licznika zegara Timer1
		TIMER_START;            // uruchomienie zegara
	}
	else
	{
		TIMER_STOP;             // zatrzymanie Timer1
		INT_FALLING;            // nastepne zbocze opadajace
		timer1 = GET_TIMER1;    // odczytanie wartosci Timer1

		bit = 0;
		if (timer1 > 20000 && timer1 < 30000)
			bit = 1;
		if (timer1 < 8000 || timer1 > 30000)
			error = 1;
		
		switch (counter)
		{
			case 0: // poczatek transmisji
				if (!stop && !error)
				{
					find_prg();
					memcpy(&time, &correct, sizeof(_rtc));
				}
				error = 0;	// poczatek transmisji, czyli na razie brak bledow
				break;
			case 21: // minuty w BCD, bity: 27:26:25:24:23:22:21
				temp_index = 0;
				parity = 0;
				break;
			case 28: // bit parzystosci dla minut
			if (parity_even_bit(temp) == bit)
				{
					correct.minute = bcd2bin(temp);
				}
				break;
			case 29: // godziny w BCD, bity: 34:33:32:31:30:29
				temp_index = 0;
				parity = 0;
				break;
			case 35: // bit parzystosci dla godzin
				if (parity == bit)
				{
					correct.hour = bcd2bin(temp);
				}
				break;
			case 36: // dni miesiaca w BCD, bity: 41:40:39:38:37:36
				temp_index = 0;
				break;
			case 42: // dni tygodnia, bity: 44:43:42
				// zapisac dni miesiaca
				temp_index = 0;
				break;
			case 45: // miesiac w BCD, bity: 49:48:47:46:45
				// zapisac dni tygodnia
				temp_index = 0;
				break;
			case 50: // lata w BCD, bity: 57:56:55:54:53:52:51:50
				// zapisac miesiac
				temp_index = 0;
				break;
			case 58: // bit parzystosci dla bitow: 36-57
				// zapisac
				break;
		}
		if (bit)
			temp |= 1<<temp_index;
		else
			temp &= ~(1<<temp_index);
		temp_index++;
		counter++;
		CLEAR_TIMER1; // wyzerowanie zegara Timer1
		TIMER_START;  // uruchomienie zegara Timer1
	}
}

/* INTERRUPT FROM SIGNAL OVERFLOW 1:
 * Przerwanie obs³ugi Timera 1 (licznik DCF)
 */
INTERRUPT(SIG_OVERFLOW1)
{
	stop++;
	if(stop >= 2)
	{
		stop = 0;
		counter = 0;
		error = 1;
	}
}
