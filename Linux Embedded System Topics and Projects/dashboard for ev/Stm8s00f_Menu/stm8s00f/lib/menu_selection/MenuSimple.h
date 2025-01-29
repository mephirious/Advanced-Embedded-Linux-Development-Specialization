#ifndef __MENUSIMPLE__
#define __MENUSIMPLE__

#include <Arduino.h>

#define UP_BUTTON           PA1
#define DOWN_BUTTON         PA3
#define SELECTION_BUTTON    PD4
#define ESC_BUTTON          PA2


void ButtonConfigrature(void);
// void create_char(void);
void ButtonCheck(void);
void ButtonProcess(char buttonPressed);
void mainMenu(void);

#endif //__MENU__