#ifndef __MENU__
#define __MENU__

#include <Arduino.h>

#define UP_BUTTON           PD4
#define DOWN_BUTTON         PD2
#define SELECTION_BUTTON    PA1
#define ESC_BUTTON          PA2


void configure_menu_button(void);
void create_char(void);
void checkButton();
void processButton(char buttonPressed);

void updateLevel_0();
void updateLevel_1 ();
void updateLevel_2 ();
void updateLevel_3();
#endif //__MENU__