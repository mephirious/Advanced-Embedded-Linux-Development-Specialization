#ifndef __MENU__
#define __MENU__

#include <Arduino.h>

#define UP_BUTTON           PD4
#define DOWN_BUTTON         PA3
#define SELECTION_BUTTON    PA1

#define BUTTON_DELAY_TIME 150
#define SEL_BUTTON_DELAY_TIME 1000 

void configure_menu_button(void);
char checkButton(void);
void processMenu(bool selection);
void buttonOperation(char button);
#endif //__MENU__