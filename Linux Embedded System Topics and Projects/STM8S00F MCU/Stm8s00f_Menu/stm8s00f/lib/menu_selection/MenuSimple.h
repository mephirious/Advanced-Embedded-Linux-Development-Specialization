#ifndef __MENUSIMPLE__
#define __MENUSIMPLE__

#include <Arduino.h>

#define UP_BUTTON           PA2
#define DOWN_BUTTON         PA1
#define SELECTION_BUTTON    PD4
//#define ESC_BUTTON          PA2


void ButtonConfigrature(void);
// void create_char(void);
char ButtonCheck(void);
void ButtonProcess(void);
void mainMenu(void);
void selectedmenu(void);
#endif //__MENU__