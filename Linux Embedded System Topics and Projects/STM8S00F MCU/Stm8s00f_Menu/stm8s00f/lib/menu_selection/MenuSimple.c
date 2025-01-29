
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <MenuSimple.h>
#include <lcd.h>


unsigned long btnPresstime = 0;

char button_pressed = 0;

unsigned char menu = 0;
unsigned char menuselection = 0;
unsigned char submenu = 0;

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

char ButtonCheck()
{

    bool debounceDone = false;
    if (millis() - btnPresstime > 150) {debounceDone = true;}
    if (digitalRead(SELECTION_BUTTON) == LOW && debounceDone == true){
        btnPresstime = millis(); debounceDone = false;
        return 'S';
    }
    if (millis() - btnPresstime > 150) {debounceDone = true;}
    if (digitalRead(UP_BUTTON) == LOW && debounceDone == true){
        btnPresstime = millis(); debounceDone = false;
        return 'U';
    }
    if (millis() - btnPresstime > 150) {debounceDone = true;}
    if (digitalRead(DOWN_BUTTON) == LOW && debounceDone == true){
        btnPresstime = millis(); debounceDone = false;
        return 'D';
    }
    return '0';
 
        // if (SELECTION_BUTTON == LOW)
        // {
        //     delay(50);
        //     return 'S';
        // }
        
        // if (UP_BUTTON == LOW)
        // {
        //     delay(50);
        //     return 'U';
        // }

        // if (!DOWN_BUTTON == LOW)
        // {
        //     delay(50);
        //     return 'D';
        // }
}

void ButtonProcess()
{

    button_pressed =  ButtonCheck();
    //lcd_print_c(button_pressed);
    switch (button_pressed)
    {

        case 'S':
            menuselection = 1;
            lcd_clear();
        break;

        case 'U':
            menu = 1;
        break;

        case 'D':
            menu = 0;    
        break;
    
    default:
        break;
    }
  
   mainMenu();

}

void mainMenu()
{

    if (menuselection == 1)
    {
        //lcd_clear();
        lcd_set_cursor(0,0);
        selectedmenu();

    }
    else
    {
        if(menu == 0){
            submenu = 0;
            lcd_set_cursor(0,0);
            lcd_print_s(">Distance KM ");
            lcd_print_u(Distance_km);
            lcd_print_c(' ');
            lcd_set_cursor(1,0);
            lcd_print_s(" Buzzer-Th ");
            lcd_print_u(Battery_Threshold);

        }

        else if (menu == 1) {
            submenu = 1;
            lcd_set_cursor(0,0);
            lcd_print_c(' ');
            lcd_set_cursor(1,0);
            lcd_print_s(">");
        }
    }
}

void selectedmenu()
{
    button_pressed =  ButtonCheck();
    //while(!button_pressed){}

        if (submenu == 0)
        {
            lcd_print_s("set in KM:");
            lcd_print_u(Distance_km);

            switch (button_pressed)
            {
            
            case 'S':
                menuselection = 0;
                mainMenu();
                break;

            case 'U':
                Distance_km ++;
                break;

            case 'D':
                Distance_km --;
                break;

            }
        }

        if (submenu == 1)
        {
            lcd_print_s("set threshold:");
            lcd_print_u(Battery_Threshold);

            switch (button_pressed)
            {
            
            case 'S':
                menuselection = 0;
                mainMenu();
                break;

            case 'U':
                Battery_Threshold ++;
                break;

            case 'D':
                Battery_Threshold --;
                break;

            }
        }
}