
#include <io.h>
#include <sig-avr.h>
#include "keyboard.h"

#define FIRST_COLUMN 0x80
#define LAST_COLUMN 0x10

	volatile TKeyboard Keyboard;

void KeyboardInit(void)
{
	TCCR0 = 3; // divide by 64
	TIMSK |= BV(TOIE0);
	KBD_DIRECTION = 0xF0;
}

unsigned int KeyboardWait(unsigned int mask)
{
	while (!Keyboard.Locked && !(Keyboard.State & mask));
	Keyboard.Pressed = Keyboard.State & mask;
	Keyboard.Key = (Keyboard.Pressed & MASK0) ? KEY0 : (Keyboard.Pressed & MASK1) ? KEY1 : (Keyboard.Pressed & MASK2) ? KEY2 : (Keyboard.Pressed & MASK3) ? KEY3 :
   	       (Keyboard.Pressed & MASK4) ? KEY4 : (Keyboard.Pressed & MASK5) ? KEY5 : (Keyboard.Pressed & MASK6) ? KEY6 : (Keyboard.Pressed & MASK7) ? KEY7 :
      	    (Keyboard.Pressed & MASK8) ? KEY8 : (Keyboard.Pressed & MASK9) ? KEY9 : (Keyboard.Pressed & MASKOK) ? KEYOK : (Keyboard.Pressed & MASKCANCEL) ? KEYCANCEL :
         	 (Keyboard.Pressed & MASKA) ? KEYA : (Keyboard.Pressed & MASKB) ? KEYB : (Keyboard.Pressed & MASKC) ? KEYC : (Keyboard.Pressed & MASKD) ? KEYD : 0xFF;
	Keyboard.State = 0;
	return Keyboard.Pressed;
}

INTERRUPT (SIG_OVERFLOW0) // at every 2ms
{
	static unsigned char key_pressed_counter = 20;
	static unsigned char key_released_counter, column = FIRST_COLUMN;
	static unsigned int  row_data, crt_key;
	TCNT0 = 0x45;     //reload counter value
	row_data <<= 4;        // get a group of 4 keys in in row_data
	row_data |= ~inp(KBD_PIN) & 0x0F;
	column >>= 1;
	if (column == (LAST_COLUMN>>1))
   {
   	column = FIRST_COLUMN;
   	if (row_data == 0)
   		goto new_key;
   	if (Keyboard.Locked)
   		Keyboard.Counter = KBD_DELAY_LOCK;
   	if (key_released_counter)
   		--key_released_counter;
   	else
      {
      	if (--key_pressed_counter == 9)
      		crt_key = row_data;
      	else
         {
         	if (row_data != crt_key)
            {
            	new_key:
            	key_pressed_counter = 10;
            	key_released_counter = 0;
            	goto end_key;
            };
         	if (!key_pressed_counter)
            {
            	Keyboard.State = row_data;
		     		Keyboard.Counter = 0;
            	key_released_counter = 20;
            };
         };
      };
   	end_key:;
			row_data=0;
     	if (Keyboard.Counter >= KBD_DELAY_LOCK)
     		Keyboard.Locked = 1;
     	else
     	{
     		Keyboard.Locked = 0;
     		Keyboard.Counter++;
     	}
   };
   KBD_PORT = ~column; // select next column, inputs will be with pull-up
}
