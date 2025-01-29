
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <MenuSimple.h>
#include <lcd.h>

char button_pressed;

unsigned int menu = 0;
unsigned int submenu = 0;
unsigned int milli = 0;

unsigned int Distance_km = 60;
unsigned int Battery_Threshold = 20;

LiquidCrystal(lcd, PD3, PD1, PC7, PC6, PC5, PC4);

void ButtonConfigrature()
{
    pinMode(SELECTION_BUTTON, INPUT);
    pinMode(UP_BUTTON, INPUT);
    pinMode(DOWN_BUTTON, INPUT);
}

void ButtonCheck()
{

    milli = millis();
    if (SELECTION_BUTTON == LOW)
    {
        milli;
        button_pressed = 'E';
    }
    if (UP_BUTTON == LOW)
    {
        milli;
        button_pressed = 'U';
    }
    if (DOWN_BUTTON == LOW)
    {
        milli;
        button_pressed = 'D';
    }

    ButtonProcess(button_pressed);
}

void ButtonProcess(char button_pressed)
{

    switch (button_pressed)
    {
    lcdclear();

        case 'E':
            menu = 0;
        break;

        case 'U':
            menu++;
        break;

        case 'D':
            menu--;    
        break;
    
    default:
        mainMenu();
        break;
    }
}

void mainMenu()
{

    if(menu == 0){

        lcd_string("<Distance KM",10);
        lcd_print_u(Distance_km);
        lcd_set_cursor(1,0);
        lcd_string(" BuzzerThreshold",16);
        lcd_print_u(Battery_Threshold);

    }

    else if(menu == 1){

        lcd_set_cursor(1,0);
        lcd_string("<",1);
    }
}