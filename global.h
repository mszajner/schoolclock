
/* Plik nag³ówkowy: global.h
 * Zawartoœæ:       Wszystkie sta³e i typy wykorzystane w projekcie.
 *                  Równie¿ konfiguracja ca³ego projektu.
 *
 * Autor:           Miros³aw Szajner
 * Klasa:           V TEL
 * Rok:             2002/2003
 *
 */
 
#ifndef GLOBAL_H
#define GLOBAL_H

/* Sta³e opisuj¹ce schemat po³¹czeñ...
 */
#define RING_PORT                     PORTD              // Port, do którego pod³¹czono dzwonek
#define RING_DIRECTION                DDRD               // Port konfiguracji portu RING_PORT
#define RING_PIN                      0                  // Numer pinu, do którego pod³¹czono dzwonek
#define DCF_PORT                      PORTD              // Port, do którego pod³¹czono DCF
#define DCF_DIRECTION                 DDRD               // Port konfiguracji portu DCF_PORT
#define DCF_PIN                       1                  // Numer pinu, do którego pod³¹czono DCF
#define KBD_PORT                      PORTB              // Port, do którego pod³¹czono klawiaturê
#define KBD_DIRECTION                 DDRB               // Port konfiguracji portu KBD_PORT
#define KBD_PIN                       PINB               // Port do odczytu portu KBD_PORT
#define LCD_PORT                      PORTA              // Port wyœwietlacza LCD
#define LCD_DIRECTION                 DDRA               // Port konfiguracji portu LCD_PORT
#define LCD_PIN_ENABLE                2                  // Numer pinu zapisu danych do wyœwietlacza
#define LCD_PIN_LIGHT                 1                  // Numer pinu do w³¹czenia podœwietlenia
#define LCD_PIN_RS                    0                  // Numer pinu wyboru zapisywanych danych
#define DIODES_PORT                   PORTC
#define DIODES_DIRECTION              DDRC
#define DIODES_BLOCKED                0

/* Sta³e opisuj¹ce wielkoœci struktur...
 */
#define PIN_LENGTH                    4                  // D³ugoœæ PINu
#define PROGRAM_LENGTH                64                 // Rozmiar jednego programu
#define PROGRAM_ITEM_LENGTH           2                  // Welkoœæ jednej pozycji w bajtach
#define PROGRAM_ITEM_MAX              15                 // Maksymalna iloœæ pozycji (lekcja + przerwa)
                                                         // w programie

/* Sta³e do opisuj¹ce adresy poszczególnych danych
 * w pamiêci EEPROM...
 */
#define EEPROM_PROGRAM_ADDRESS        256                // Adres startu programów w EEPROM'ie
#define EEPROM_PROGRAM_NUMBER         1                  // Adres aktualnie ustawionego programu
#define EEPROM_PROGRAM_CALENDAR       2                  // Adres z kalendarzem tygodniowym
#define EEPROM_PROGRAM_ENABLED        3                  // Adres z informacj¹ czy obs³uga w³¹czona
#define EEPROM_PIN                    4                  // Adres z kodem PIN

/* Sta³e opisuj¹ce opóŸnienia...
 */
#define KBD_DELAY_LOCK                4000               // OpóŸnienie blokady klawiatury
#define LCD_DELAY_MAX                 8192               // Najwiêksze opóŸnienie w transmisji do LCD
#define LCD_DELAY_16ms                2048               // OpóŸnienie wzorcowe: 16 ms
#define LCD_DELAY_4ms                 LCD_DELAY_16ms/4   // OpóŸnienie 4 ms
#define LCD_DELAY_1ms                 LCD_DELAY_4ms/4    // OpóŸnienie 1 ms
#define LCD_DELAY_200us               LCD_DELAY_1ms/5    // OpóŸnienie 200 us
#define LCD_DELAY_100us               LCD_DELAY_1ms/10   // OpóŸnienie 100 us

/* Sta³e opisuj¹ce klawiaturê...
 */
#define KEY0                          0                  // Reprezentacja numeryczna klawisza 0
#define KEY1                          1                  // Reprezentacja numeryczna klawisza 1
#define KEY2                          2                  // Reprezentacja numeryczna klawisza 2
#define KEY3                          3                  // Reprezentacja numeryczna klawisza 3
#define KEY4                          4                  // Reprezentacja numeryczna klawisza 4
#define KEY5                          5                  // Reprezentacja numeryczna klawisza 5
#define KEY6                          6                  // Reprezentacja numeryczna klawisza 6
#define KEY7                          7                  // Reprezentacja numeryczna klawisza 7
#define KEY8                          8                  // Reprezentacja numeryczna klawisza 8
#define KEY9                          9                  // Reprezentacja numeryczna klawisza 9
#define KEYOK                         10                 // Reprezentacja numeryczna klawisza OK
#define KEYCANCEL                     11                 // Reprezentacja numeryczna klawisza ANULUJ
#define KEYA                          12                 // Reprezentacja numeryczna klawisza A
#define KEYB                          13                 // Reprezentacja numeryczna klawisza B
#define KEYC                          14                 // Reprezentacja numeryczna klawisza C
#define KEYD                          15                 // Reprezentacja numeryczna klawisza D
#define MASK0                         BV(7)              // Maska klawisza 0
#define MASK1                         BV(2)              // Maska klawisza 1
#define MASK2                         BV(6)              // Maska klawisza 2
#define MASK3                         BV(10)             // Maska klawisza 3
#define MASK4                         BV(1)              // Maska klawisza 4
#define MASK5                         BV(5)              // Maska klawisza 5
#define MASK6                         BV(9)              // Maska klawisza 6
#define MASK7                         BV(0)              // Maska klawisza 7
#define MASK8                         BV(4)              // Maska klawisza 8
#define MASK9                         BV(8)              // Maska klawisza 9
#define MASKOK                        BV(3)              // Maska klawisza OK
#define MASKCANCEL                    BV(11)             // Maska klawisza ANULUJ
#define MASKA                         BV(12)             // Maska klawisza A
#define MASKB                         BV(13)             // Maska klawisza B
#define MASKC                         BV(14)             // Maska klawisza C
#define MASKD                         BV(15)             // Maska klawisza D
#define MASK_NUMERIC    MASK0 | MASK1 | MASK2 | MASK3 \
                      | MASK4 | MASK5 | MASK6 | MASK7 \
                      | MASK8 | MASK9                    // Maska klawiszy numerycznych
#define MASK_SPECIAL    MASKA | MASKB | MASKC | MASKD    // Maska klawiszy specjalnych A,B,C,D
#define MASK_CONTROL    MASKOK | MASKCANCEL              // Maska klawiszy kontroli OK i ANULUJ
#define MASK_ALL        MASK_NUMERIC | MASK_SPECIAL\
                      | MASK_CONTROL                     // Maska dla wszystkich klawiszy

/* Makra wykorzystywane w module i projekcie,
 * do w³¹czania/wy³¹czania dzwonka, synchronizacji DCF
 * i inne...
 */
#define RingOn()            sbi(RING_PORT, RING_PIN)     // Makro do w³¹czania dzwonka
#define RingOff()           cbi(RING_PORT, RING_PIN)     // Makro do wy³¹czania dzwonka
#define DcfOn()             sbi(DCF_PORT, DCF_PIN)       // Makro do w³¹czania synchronizacji
#define DcfOff()            cbi(DCF_PORT, DCF_PIN)       // Makro do wy³¹czania synchronizacji
#define DcfInit()           sbi(DCF_DIRECTION,DCF_PIN)   // Pin w³¹czania DCF wyjœciowy
#define RingInit()          sbi(RING_DIRECTION,RING_PIN) // Pin w³¹czania dzwonka wyjœciowy
#define IsLesson            (!(Program.Index % 2))       // > 0, jeœli trwa lekcja

/* TKeyboard: przechowuje informacje o stanie klawiatury...
 */
typedef struct {
	unsigned char Key;                                    // Aktualnie odczytany klawisz
	unsigned int  Pressed;                                // Stan wszystkich klawiszy po odczycie klawiatury
	unsigned int  State;                                  // Atkualny stan wszystkich klawiszy
	unsigned char Locked;                                 // > 0, jeœli klawiatura zablokowana
	unsigned int  Counter;                                // Licznik blokady klawiatury
} TKeyboard;

/* TPin: przechowuje informacje o podanym kodzie PIN
 */
typedef struct {
	unsigned char Count;                                  // Iloœæ wpisanych znaków
	unsigned char Pin[PIN_LENGTH];                        // Podany kod PIN
 	unsigned char Old[PIN_LENGTH];                        // Kod PIN odczytany z EEPROMu
} TPin;

/* TItems: przechowuje informacje
 * o wszystkich pozycjach programu...
 */
typedef struct {
	unsigned char Count;                                  // Iloœæ pozycji programu
	unsigned char Items[PROGRAM_ITEM_MAX]                 // Kolejne pozycje programu
	                   [PROGRAM_ITEM_LENGTH]; 
} TItems;

/* TTime: przechowuje informacje o czasie...
 */
typedef struct {
   unsigned char Changed;                                // > 0, jeœli czas uleg³ zmianie
   unsigned char Hour;                                   // Godzina
   unsigned char Minute;                                 // Minuta
   unsigned char Second;                                 // Sekunda
   unsigned char DayOfWeek;                              // Dzieñ tygodnia
   unsigned char Day;                                    // Dzieñ miesi¹ca
   unsigned char Month;                                  // Miesi¹c
   unsigned int  Year;                                   // Rok
} TTime;

/* TProgram: przechowuje informacje o programie...
 */
typedef struct {                                         // Struktura definiuje program
   unsigned char Number;                                 // Numer wybranego programu
   unsigned char Calendar;                               // Kalendarz tygodniowy
   unsigned char Enabled;                                // > 0, jeœli obs³uga programów w³.
   unsigned char Hour;                                   // Godzina nastêpnego dzwonienia
   unsigned char Minute;                                 // Minuta nastêpnego dzwonienia
   unsigned char Count;                                  // Iloœæ pozycji w programie
   char Index;                                           // Numer aktualnie trwaj¹cej pozycji
} TProgram;

/* TStauts: okreœla status zegara
 */
typedef enum {
	Reset,                                                // Wyst¹pi³ reset uk³adu
	Power,                                                // Ponowne w³¹czenie napiêcia sieciowego
	Normal,                                               // Przejœcie do blokady klawiatury zegara
	Locked,                                               // Blokada klawiatury zegara (wyœw. czasu)
	Operating,                                            // Przejœcie do odblokowywania klaw. zegara
	Unlock,                                               // Klawiatura odblokowana, sprawdzenie PINu
	Menu,                                                 // Wyœwietlenie i obs³uga menu zegara
} TStatus;

#endif
