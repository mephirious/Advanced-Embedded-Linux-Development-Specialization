#include <Arduino.h>
#include "menu.h"
#include "LiquidCrystal.h"
#include "EEPROM.h"

extern LiquidCrystal (lcd,PD3,PD2,PD1,PC7,PC6,PC5,PC4);
unsigned long btnPresstime = 0;
uint8_t main_dis = 0; uint8_t level = 0;
uint8_t km_range = 60;

int addr = 0;

void configure_menu_button(void)
{
    pinMode(UP_BUTTON, INPUT);
    pinMode(DOWN_BUTTON, INPUT);
    pinMode(SELECTION_BUTTON, INPUT);
}

char checkButton()
{
    bool debounceDone = false;
    if (millis() - btnPresstime > SEL_BUTTON_DELAY_TIME) {debounceDone = true;}
    if (digitalRead(SELECTION_BUTTON) == LOW && debounceDone == true){
        btnPresstime = millis(); debounceDone = false;
        return 'S';
    }
    if (millis() - btnPresstime > BUTTON_DELAY_TIME) {debounceDone = true;}
    if (digitalRead(UP_BUTTON) == LOW && debounceDone == true){
        btnPresstime = millis(); debounceDone = false;
        return 'U';
    }
    if (millis() - btnPresstime > BUTTON_DELAY_TIME) {debounceDone = true;}
    if (digitalRead(DOWN_BUTTON) == LOW && debounceDone == true){
        btnPresstime = millis(); debounceDone = false;
        return 'D';
    }
    return '0';
}
void processMenu(bool selection)
{
    if(level == 1){
        lcd_clear();
        lcd_print_s("Set KM Range:");
        lcd_setCursor(0,1);
        lcd_print_u(km_range);
        lcd_setCursor(13,1);
        lcd_write(1);
    }
    if (level == 2)
    {
        lcd_setCursor(0,1);
        lcd_print_u(km_range);
        lcd_setCursor(12,1);
        lcd_write(0);

        if(km_range < 10){
            lcd_setCursor(1,1);
            lcd_print_c(' ');
        }
        else if(km_range < 100){
            lcd_setCursor(2,1);
            lcd_print_c(' ');
        }
        delay(50);
    }
}

void buttonOperation(char button){
    switch(main_dis)
    {
        case 0:
        switch(button)
        {
            case 'S':
            lcd_clear();
            main_dis = 1;
            level = 1;
            processMenu(false);
            break;
        }
        break;

        case 1:
        switch(button)
        {
            case 'S':
            if (level == 2){
                EEPROM_write(addr, km_range);
                lcd_clear();
                level = 0; main_dis = 0;
            }
            else{level = 2;}
            processMenu(false);
            break;

            case 'U':
            if (km_range < 255){km_range++;}
            else{km_range = 255;}
            processMenu(false);
            break;

            case 'D':
            if (km_range > 0){km_range--;}
            else{km_range = 0;}
            processMenu(false);
            break;
        }

        break;
    }
        
}
