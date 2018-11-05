
/* Modu³:      KEYBOARD
 *             Klawiatura matrycowa 16-klawiszy
 * Autor:      Miros³aw Szajner
 * Klasa:      V TEL
 * Rok:        2002/2003
 * Copyright:  Miros³aw Szajner
 *
 * Modu³ wykorzystuje Timer 0 do sprawdzania stanu klawiszy.
 */

#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "global.h"

extern volatile TKeyboard    Keyboard;                         // Struktura ze stanem klawiatury
extern void                  KeyboardInit(void);               // Inicjalizacja klawiatury
extern unsigned int          KeyboardWait(unsigned int mask);  // Odczytywanie klawiszy

#endif
