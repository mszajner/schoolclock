
/* Miroslaw Szajner 
 * kerimirek@tlen.pl
 * 2002/2003
 */

#include <io8535.h>
#include <string.h>
#include <sig-avr.h>
#include <wdt.h>
#include <eeprom.h>
#include <pgmspace.h>
#include "rtc.h"
#include "keyboard.h"
#include "lcd.h"
#include "global.h"
#include "strings.h"

#define int_cast     (int)
#define uint_cast    (unsigned int)
#define chars_cast   (char*)
#define char_cast    (char)
#define uchars_cast  (unsigned char*)
#define uchar_cast   (unsigned char)

#define LCD_LIGHT   0
#define LCD_GOTOXY  0
//#define DEBUG

volatile TStatus Status;
volatile TPin    Pin;
volatile TTime   NewTime;
volatile TItems  Items;

char EnterPin(unsigned char *Msg)
{
	Pin.Count = 0;
	while (!Keyboard.Locked)
	{
		LcdPrintf(Msg);
		LcdChars('*', Pin.Count);
		if (KeyboardWait(MASK_NUMERIC | MASKOK | MASKCANCEL))
		{
			if (Keyboard.Key == KEYOK)
			{
				if (Pin.Count == 4)
				{
					eeprom_read_block(uchars_cast(Pin.Old), EEPROM_PIN, PIN_LENGTH);
					return memcmp(uchars_cast(Pin.Pin), uchars_cast(Pin.Old), PIN_LENGTH) ? 2 : 1;
				}
			}
			else if (Keyboard.Key == KEYCANCEL)
			{
				if (!Pin.Count)
					break;
				Pin.Count--;
			}
			else if (Pin.Count < 4)
			{
				Pin.Pin[Pin.Count] = Keyboard.Key;
				Pin.Count++;
			}
		}
	}
	return 0;
}

char EnterTime(unsigned char *Format, unsigned char Length)
{
	#if (LCD_GOTOXY == 1)
	static unsigned char XPos[] PROGMEM = { 0, 1, 3, 4, 6, 7 };
	#endif
	unsigned char Index = 0;
	while(!Keyboard.Locked)
	{
		LcdPrintf(Format, NewTime.Hour, NewTime.Minute, NewTime.Second);
		#if (LCD_GOTOXY == 1)
		LcdGotoxy(PRG_RDB(XPos + Index), 1);
		#endif
		if (KeyboardWait(MASK_NUMERIC | MASKCANCEL | MASKOK))
		{
			if (Keyboard.Key == KEYCANCEL)
			{
				if (!Index)
					break;
				Index--;
			}
			else if (Keyboard.Key == KEYOK)
			{
				if (NewTime.Changed == Length)
					break;
			}
			else
			{
				switch (Index)
				{
					case 0:
						if (Keyboard.Key < 3)
						{
							NewTime.Hour = Keyboard.Key*10 + NewTime.Hour % 10;
							Index++;
						}
						break;
					case 1:
						if (Keyboard.Key < 4 || (NewTime.Hour / 10) < 2)
						{
							NewTime.Hour = Keyboard.Key + (NewTime.Hour / 10)*10;
							Index++;
						}
						break;
					case 2:
						if (Keyboard.Key < 6)
						{
							NewTime.Minute = Keyboard.Key*10 + NewTime.Minute % 10;
							Index++;
						}
						break;
					case 3:
						NewTime.Minute = Keyboard.Key + (NewTime.Minute / 10)*10;
						Index++;
						break;
					case 4:
						if (Keyboard.Key < 6)
						{
							NewTime.Second = Keyboard.Key*10 + NewTime.Second % 10;
							Index++;
						}	
						break;
					case 5:
						NewTime.Second = Keyboard.Key + (NewTime.Second / 10)*10;
						Index++;
						break;
				}
			}
		}
	}
	NewTime.Changed = Index;
	return NewTime.Changed;
}

char EnterDate(unsigned char *Format)
{
	#if (LCD_GOTOXY == 1)
	static unsigned char XPos[] PROGMEM = { 0, 3, 4, 6, 7, 11, 12 };
	#endif
	NewTime.Changed = 0;
	while(!Keyboard.Locked)
	{
		LcdPrintf(Format, STR_DAYS[NewTime.DayOfWeek], NewTime.Day, NewTime.Month, NewTime.Year % 100);
		#if (LCD_GOTOXY == 1)
		LcdGotoxy(PRG_RDB(XPos + NewTime.Changed), 1);
		#endif
		if (KeyboardWait(MASK_NUMERIC | MASKCANCEL | MASKOK))
		{
			if (Keyboard.Key == KEYCANCEL)
			{
				if (!NewTime.Changed)
					break;
				NewTime.Changed--;
			}
			else if (Keyboard.Key == KEYOK)
			{
				if (NewTime.Changed == 7)
					break;
			}
			else
			{
				switch (NewTime.Changed)
				{
					case 0:
						if (Keyboard.Key < 7)
						{
							NewTime.DayOfWeek = Keyboard.Key;
							NewTime.Changed++;
						}
						break;
					case 1:
						if (Keyboard.Key < 4)
						{
							NewTime.Day = (10 * Keyboard.Key) + (NewTime.Day % 10);
							NewTime.Changed++;
						}
						break;
					case 2:
						if (Keyboard.Key < 2 || (NewTime.Day / 10) < 3)
						{
							NewTime.Day = Keyboard.Key + (NewTime.Day / 10) * 10;
							NewTime.Changed++;
						}
						break;
					case 3:
						if (Keyboard.Key < 2)
						{
							NewTime.Month = (10*Keyboard.Key) + (NewTime.Month % 10);
							NewTime.Changed++;
						}
						break;
					case 4:
						if (Keyboard.Key < 3 || (NewTime.Month / 10) < 1)
						{
							NewTime.Month = Keyboard.Key + (NewTime.Month / 10) * 10;
							NewTime.Changed++;
						}	
						break;
					case 5:
						NewTime.Year = 2000 + (10*Keyboard.Key) + (NewTime.Year % 10);
						NewTime.Changed++;
						break;
					case 6:
						NewTime.Year = 2000 + Keyboard.Key + (NewTime.Year / 10) * 10;
						NewTime.Changed++;
						break;
				}
			}
		}
	}
	return NewTime.Changed;
}

inline char ModifyProgram(unsigned int Address)
{
	Items.Count = 0;
	Address++;
	while (!Keyboard.Locked)
	{
		LcdPrintf(uchars_cast(STR_PROGRAM_ENTER_ITEM), uchars_cast((Items.Count+1)%2 ? STR_PROGRAM_ITEM_START : STR_PROGRAM_ITEM_STOP), Items.Count+1);
		NewTime.Hour = eeprom_rb(Address++);
		NewTime.Minute = eeprom_rb(Address++);
		if (EnterTime(uchars_cast(STR_PROGRAM_ENTER_TIME), 4))
		{
			Items.Items[Items.Count][0] = NewTime.Hour;
			Items.Items[Items.Count][1] = NewTime.Minute;
			Items.Count++;
			if (Items.Count == (PROGRAM_ITEM_MAX<<1))
				return 1;
		}
		else
		{
			if (!Items.Count)
				return 0;
			Items.Count--;
			Address -= 2;
		}
	}
	return 0;
}

int main(void)
{
	Status = Reset;
	while(1)
	{
		switch (Status)
		{
			case Reset:
				RtcInit();
				ProgramLoad();
				KeyboardInit();
				DcfEnable();
				memset(uchars_cast(&Time), 0, sizeof(TTime));
				memset(uchars_cast(&Keyboard), 0, sizeof(TKeyboard));
				GIMSK |= BV(INT1);
				MCUCR |= BV(ISC10) | BV(ISC11);
				asm("sei");

			case Power:
				LcdInit();
				Status = Normal;

			case Normal:
				#if (LCD_LIGHT == 1)
				LcdLightOff();
				#endif
				LcdDisplay(1, 0, 0);
				Status = Locked;
				Keyboard.Locked = 1;
				break;
			
			case Operating:
				#if (LCD_LIGHT == 1)
				LcdLightOn();
				#endif
				LcdDisplay(1, 1, 0);
				Status = Unlock;

			case Unlock:
				Status = EnterPin(uchars_cast(STR_ENTER_PIN)) == 1 ? Menu : Normal;
				break;

			default:
			case Locked:
				if (Time.Changed)
				{
					LcdPrintf(uchars_cast(STR_FORMAT_TIME), uchars_cast(STR_DAYS[Time.DayOfWeek]), Time.Hour, Time.Minute, Time.Second);
					if (Program.Index > -1)
						LcdPrintf(uchars_cast(STR_FORMAT_PROGRAM), uchars_cast(IsLesson ? STR_LESSON : STR_BREAK), (Program.Index/2)+1);
					Time.Changed = 0;
				}
				if (!Keyboard.Locked)
				{
					Status = Operating;
				}
				break;
			
			case Menu:
				LcdPrintf(uchars_cast(STR_MENU));
				if (KeyboardWait(MASK_NUMERIC))
				{
					switch (Keyboard.Key)
					{
						default:
						case KEY0:
							Status = Normal;
							break;

						case KEY1:
							memcpy(uchars_cast(&NewTime), uchars_cast(&Time), sizeof(TTime));
							if (EnterTime(uchars_cast(STR_FORMAT_ENTER_TIME), 6))
							{
								memcpy(uchars_cast(&Time.Hour), uchars_cast(&NewTime.Hour), 3);
								LcdPrintf(uchars_cast(STR_WRITTEN));
							}
							break;

						case KEY2:
							memcpy(uchars_cast(&NewTime), uchars_cast(&Time), sizeof(TTime));
							if (EnterDate(uchars_cast(STR_FORMAT_ENTER_DATE)))
							{
								memcpy(uchars_cast(&Time.DayOfWeek), uchars_cast(&NewTime.DayOfWeek), 5);
								LcdPrintf(uchars_cast(STR_WRITTEN));
							}
							break;

						case KEY3:
							LcdPrintf(uchars_cast(STR_CHOOSE_PROGRAM));
							if (KeyboardWait(MASK_SPECIAL | MASKCANCEL) & (~MASKCANCEL))
							{
								unsigned char i;
								unsigned int Address = (Keyboard.Key - KEYA) * PROGRAM_LENGTH;
								Address += EEPROM_PROGRAM_ADDRESS;
								if (ModifyProgram(Address))
								{
									eeprom_wb(Address++, Items.Count);
									for (i=0; i<PROGRAM_ITEM_MAX; i++)
									{
										eeprom_wb(Address++, Items.Items[i][0]);
										eeprom_wb(Address++, Items.Items[i][1]);
									}
									LcdPrintf(uchars_cast(STR_WRITTEN));
								}
							}
							break;

						case KEY4:
							LcdPrintf(uchars_cast(STR_CHOOSE_PROGRAM));
							if (KeyboardWait(MASK_SPECIAL | MASKCANCEL) & (~MASKCANCEL))
							{
								eeprom_wb(EEPROM_PROGRAM_NUMBER, Keyboard.Key-KEYA);
								LcdPrintf(uchars_cast(STR_FORMAT_WRITTEN_PROGRAM), Keyboard.Key + 'A' - KEYA);
							}
							break;

						case KEY5:
							if (EnterPin(uchars_cast(STR_ENTER_PIN)) == 1)
								if (EnterPin(uchars_cast(STR_ENTER_NEW_PIN)) == 2)
								{
									unsigned char i;
									for (i=0; i<4; i++)
										eeprom_wb(0x04 + i, Pin.Pin[i]);
									LcdPrintf(uchars_cast(STR_WRITTEN));
								}
							break;

						case KEY6:
							LcdPrintf(uchars_cast(STR_FORMAT_ENABLE_RING), uchars_cast(Program.Enabled ? STR_TURN_OFF : STR_TURN_ON));
							if (KeyboardWait(MASK_CONTROL) & MASKOK)
							{
								Program.Enabled = Program.Enabled ? 0 : 1;
								eeprom_wb(EEPROM_PROGRAM_ENABLED, Program.Enabled);
								LcdPrintf(uchars_cast(STR_FORMAT_RING_STATUS), Program.Enabled ? chars_cast(STR_TURN_ON+1) : chars_cast(STR_TURN_OFF+1));
							}
							break;

						case KEY7:
							LcdPrintf(uchars_cast(STR_PROGRAM_RELOAD));
							if (KeyboardWait(MASKOK))
							{
								ProgramLoad();
								LcdPrintf(uchars_cast(STR_LOADED));
							}
							break;

						case KEY8:
						{
							unsigned char i = 0;
							while (!Keyboard.Locked)
							{
								LcdPrintf(uchars_cast(STR_FORMAT_MODIFY_CALENDAR), uchars_cast((Program.Calendar & BV(i)) ? STR_TURN_OFF : STR_TURN_ON), uchars_cast(STR_DAYS[i]));
								if (KeyboardWait(MASK_CONTROL) & MASKOK)
								{
									if (Program.Calendar & BV(i))
										Program.Calendar &= ~BV(i);
									else
										Program.Calendar |= BV(i);
								}
								i++; // increase counter
								if (i == 7) // if counter is equal 7
								{
									eeprom_wb(EEPROM_PROGRAM_CALENDAR, Program.Calendar);
									LcdPrintf(uchars_cast(STR_WRITTEN));
									break;
								}
							}
						}
							break;

						case KEY9:
							LcdPrintf(uchars_cast(STR_ABOUT));
							break;
					}
				}
				else
					Status = Normal;
				break;
		}
	}
}

INTERRUPT (SIG_INTERRUPT1)
{
	Status = Power;
}
