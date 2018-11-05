
#ifndef STRINGS_H
#define STRINGS_H

static unsigned char STR_FORMAT_ENTER_TIME[]           PROGMEM = ("%~Podaj czas:%n%b:%b:%b");
static unsigned char STR_FORMAT_ENTER_DATE[]           PROGMEM = ("%~Podaj date:%n%f, %b/%b/20%b");
static unsigned char STR_FORMAT_MODIFY_CALENDAR[]      PROGMEM = ("%~%f. %f?");
static unsigned char STR_FORMAT_ENABLE_RING[]          PROGMEM = ("%~%f. dzwonek?");
static unsigned char STR_FORMAT_RING_STATUS[]          PROGMEM = ("%~Dzwonek w%f.!%*");
static unsigned char STR_FORMAT_WRITTEN_PROGRAM[]      PROGMEM = ("%nZapisano %c!%*");
static unsigned char STR_FORMAT_TIME[]                 PROGMEM = ("%~  %f, %b:%b:%b");
static unsigned char STR_FORMAT_PROGRAM[]              PROGMEM = ("%n   %f %d");
static unsigned char STR_PROGRAM_ENTER_ITEM[]          PROGMEM = ("%~%f lekcji %d%n");
static unsigned char STR_PROGRAM_ENTER_TIME[]          PROGMEM = ("%b:%b%l");

static unsigned char STR_DAYS[7][3]                    PROGMEM = { "Pn", "Wt", "Sr", "Cz", "Pt", "So", "Nd" };
static unsigned char STR_PROGRAM_RELOAD[]              PROGMEM = ("%~Czy przeladowac%nprogram?");
static unsigned char STR_LOADED[]                      PROGMEM = ("%~Zaladowano!%*");
static unsigned char STR_ENTER_PIN[]                   PROGMEM = ("%~Podaj PIN: ");
static unsigned char STR_MENU[]                        PROGMEM = ("%~Menu...");
static unsigned char STR_WRITTEN[]                     PROGMEM = ("%~Zapisano!%*");
static unsigned char STR_CHOOSE_PROGRAM[]              PROGMEM = ("%~Wybierz program:");
static unsigned char STR_ENTER_NEW_PIN[]               PROGMEM = ("%~Nowy PIN: ");
static unsigned char STR_TURN_ON[]                     PROGMEM = ("Wlacz");
static unsigned char STR_TURN_OFF[]                    PROGMEM = ("Wylacz"); 
static unsigned char STR_LESSON[]                      PROGMEM = (" Lekcja");
static unsigned char STR_BREAK[]                       PROGMEM = ("Przerwa");
static unsigned char STR_PROGRAM_ITEM_START[]          PROGMEM = ("Pocz.");
static unsigned char STR_PROGRAM_ITEM_STOP[]           PROGMEM = ("Koniec");
static unsigned char STR_ABOUT[]                       PROGMEM = ("%~ Zegar szkolny%n   Wersja 1.0%*%~Wykonal:%nMiroslaw Szajner%*");

#endif
