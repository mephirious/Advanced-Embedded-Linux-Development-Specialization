#include <Arduino.h>
#include "menu.h"
#include <LiquidCrystal.h>
LiquidCrystal(lcd, PD3, PD1, PC7, PC6, PC5, PC4);
byte charUp[8] = {
    B00100,
    B01110,
    B11111,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000};
byte charDown[8] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B11111,
    B01110,
    B00100};
byte charUpDown[8] = {
    B00100,
    B01110,
    B11111,
    B00000,
    B00000,
    B11111,
    B01110,
    B00100};

char buttonPressed = '0';
byte menuLevel = 0; 
byte menu = 0;
byte sub = 1;

uint8_t batteryAttachedBMScap = 100;
uint8_t battLowThreshold = 15;
uint8_t speedMode = 0;

bool currState_B = HIGH;
bool currState_D = HIGH;
bool currState_U = HIGH;
bool currState_E = HIGH;
bool prevState_B = HIGH;
bool prevState_D = HIGH;
bool prevState_U = HIGH;
bool prevState_E = HIGH;
unsigned long prevTime_B = 0;
unsigned long prevTime_D = 0;
unsigned long prevTime_U = 0;
unsigned long prevTime_E = 0;
unsigned long waitTime_B = 50;
unsigned long waitTime_D = 50;
unsigned long waitTime_U = 50;
unsigned long waitTime_E = 50;    

void configure_menu_button(void)
{
    pinMode(UP_BUTTON, INPUT);
    pinMode(DOWN_BUTTON, INPUT);
    pinMode(SELECTION_BUTTON, INPUT);
    pinMode(ESC_BUTTON, INPUT);
}

void create_char(void)
{
    lcd_createChar(0, charUp);
    lcd_createChar(1, charDown);
    lcd_createChar(2, charUpDown);
}
void checkButton()
{
    // Button Debouncing
    bool currRead_B = digitalRead(ESC_BUTTON);
    bool currRead_D = digitalRead(DOWN_BUTTON);
    bool currRead_U = digitalRead(UP_BUTTON);
    bool currRead_E = digitalRead(SELECTION_BUTTON);
    if (currRead_B != prevState_B)
    {
        prevTime_B = millis();
    }
    if (currRead_D != prevState_D)
    {
        prevTime_D = millis();
    }
    if (currRead_U != prevState_U)
    {
        prevTime_U = millis();
    }
    if (currRead_E != prevState_E)
    {
        prevTime_E = millis();
    }
    if ((millis() - prevTime_B) > waitTime_B)
    {
        if (currRead_B != currState_B)
        {
            currState_B = currRead_B;
            if (currState_B == LOW)
            {
                buttonPressed = 'B';
            }
        }
    }
    else
        buttonPressed = '0';
    if ((millis() - prevTime_D) > waitTime_D)
    {
        if (currRead_D != currState_D)
        {
            currState_D = currRead_D;
            if (currState_D == LOW)
            {
                buttonPressed = 'D';
            }
        }
    }
    else
        buttonPressed = '0';
    if ((millis() - prevTime_U) > waitTime_U)
    {
        if (currRead_U != currState_U)
        {
            currState_U = currRead_U;
            if (currState_U == LOW)
            {
                buttonPressed = 'U';
            }
            else
            {
                // buttonPressed = '0';
            }
        }
    }
    else
        buttonPressed = '0';
    if ((millis() - prevTime_E) > waitTime_E)
    {
        if (currRead_E != currState_E)
        {
            currState_E = currRead_E;
            if (currState_E == LOW)
            {
                buttonPressed = 'E';
            }
        }
    }
    else
        buttonPressed = '0';
    prevState_B = currRead_B;
    prevState_D = currRead_D;
    prevState_U = currRead_U;
    prevState_E = currRead_E;
    processButton(buttonPressed);
}
void processButton(char buttonPressed)
{
    switch (menuLevel)
    {
    case 0: // Level 0, home screen
        switch (buttonPressed)
        {
        case 'E': // Enter
            menu = 1;
            menuLevel = 1; // go to main menu
            updateLevel_1(); // show main menu
            
            break;
        case 'U': // Up
            break;
        case 'D': // Down
            break;
        case 'B': // Back
            menuLevel = 0; // go to home screen
            updateLevel_0(); // show home screen
            
            break;
        default:
            break;
        }
        break;
    case 1: // Level 1, main menu
        switch (buttonPressed)
        {
        case 'E': // Enter
            sub = 1;
            menuLevel = 2; // go to sub menu
            updateLevel_2(); // show sub menu
            
            break;
        case 'U': // Up
            menu++;
            updateLevel_1(); // show main menu
            
            break;
        case 'D': // Down
            menu--;
            updateLevel_1(); // show main menu
            
            break;
        case 'B': // Back
            menuLevel = 0; // hide menu, go back to level 0
            updateLevel_0(); // show home screen
            
            break;
        default:
            break;
        }
        break;
    case 2: // Level 2, sub menu
        switch (buttonPressed)
        {
        case 'E': // Enter
            if (sub == 2)
            { // Jump to sub menu of sub menu only when edit is necessary
                menuLevel = 3; // go to sub menu of sub menu
                updateLevel_3(); // show sub menu of sub menu
            }
            break;
        case 'U': // Up
            if (sub < 2)
            {
                sub++;
            }
            else
            {
                sub =2;
            }
            updateLevel_2();
            
            break;
        case 'D': // Down
            if (sub > 1)
            {
                sub--;
            }
            else
            {
                sub = 1;
            }
            updateLevel_2(); // show main menu
            
            break;
        case 'B': // Back
            menuLevel = 1; // go back to level 1
            updateLevel_1(); // show main menu
            
            break;
        default:
            break;
        }
        break;
    case 3: // Level 3, sub menu of sub menu
        switch (buttonPressed)
        {
        case 'E': // Enter
            menuLevel = 2; // go back to level 2
            updateLevel_2(); // show sub menu
            
            break;
        case 'U': // Up
            if (sub == 2)
            { // edit value
                if (menu == 1)
                {
                    if (batteryAttachedBMScap < 1000)
                    {
                        batteryAttachedBMScap++;
                    }
                }
                else if (menu == 2)
                {
                    if (battLowThreshold < 100)
                    {
                        battLowThreshold++;
                    }
                    else
                    {
                        battLowThreshold = 0;
                    }
                }
                else if (menu == 3)
                {
                    if (speedMode < 3)
                    {
                        speedMode++;
                    }
                    else
                    {
                        speedMode = 3;
                    }
                }
            }
            updateLevel_3(); // show sub menu
            
            break;
        case 'D': // Down
            if (sub == 2)
            { // edit value
                if (menu == 1)
                {
                    if (batteryAttachedBMScap > 0)
                    {
                        batteryAttachedBMScap--;
                    }
                    else
                    {
                        batteryAttachedBMScap = 0;
                    }
                }
                else if (menu == 2)
                {
                    if (battLowThreshold > 0)
                    {
                        battLowThreshold--;
                    }
                    else
                    {
                        battLowThreshold = 0;
                    }
                }
                else if (menu == 3)
                {
                    if (speedMode > 0)
                    {
                        speedMode--;
                    }
                    else
                    {
                        speedMode = 0;
                    }
                }
            }
            updateLevel_3(); // show sub menu
            
            break;
        case 'B': // Back
            menuLevel = 2; // go back to main menu
            updateLevel_2(); // show main menu
            
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}
void updateLevel_0()
{
    lcd_clear();
    lcd_print_s("Returning Main..");
}
void updateLevel_1()
{
    switch (menu)
    {
    case 0:
        menu = 1;
        break;
    case 1:
        lcd_clear();
        lcd_print_s(">Batt.Capacity");
        lcd_setCursor(0, 1);
        lcd_print_s(" Batt.Theshold ");
        lcd_setCursor(15, 1);
        lcd_write((byte)1); // down arrow
        break;
    case 2:
        lcd_clear();
        lcd_print_s(" Batt.Capacity");
        lcd_setCursor(0, 1);
        lcd_print_s(">Batt.Theshold ");
        lcd_setCursor(15, 1);
        lcd_write((byte)2); // up and down arrow
        break;
    case 3:
        lcd_clear();
        lcd_print_s(">Speed Modes");
        lcd_setCursor(15, 1);
        lcd_write((byte)0); // up arrow
        break;
    case 4:
        menu = 3;
        break;
    }
}
void updateLevel_2()
{
    switch (menu)
    {
    case 0:
        break;
    case 1: // Battery Capcity.
        switch (sub)
        {
        case 0:
            break;
        case 1:
            lcd_clear();
            lcd_setCursor(0, 0);
            lcd_print_s("Batt.CAP:");
            lcd_setCursor(9, 0);
            lcd_print_u(batteryAttachedBMScap);
            lcd_setCursor(14, 0);
            lcd_print_s("Ah");
            lcd_setCursor(15, 1);
            lcd_write((byte)1); // down arrow
            break;
        case 2:
            lcd_clear();
            lcd_print_s("S To EDIT->");
            lcd_setCursor(0, 1);
            lcd_print_s("Batt.CAP: ");
            lcd_setCursor(9, 1);
            lcd_print_u(batteryAttachedBMScap);
            lcd_setCursor(13, 1);
            lcd_print_s("Ah");
            lcd_setCursor(15, 1);
            lcd_write((byte)0); // up
            break;
        default:
            break;
        }
        break;
    case 2: // Relay 2
        switch (sub)
        {
        case 0:
            break;
        case 1:
            lcd_clear();
            lcd_print_s("Batt.Low In:%");
            lcd_setCursor(0, 1);
            lcd_print_s("Curr.Low:");
            lcd_setCursor(10, 1);
            lcd_print_u(20);
            lcd_print_s("%");
            lcd_setCursor(15, 1);
            lcd_write((byte)1); // down arrow
            break;
        case 2:
            lcd_clear();
            lcd_print_s("S To EDIT->");
            lcd_setCursor(0, 1);
            lcd_print_s("Curr.Low:");
            lcd_setCursor(10, 1);
            lcd_print_u(20);
            lcd_print_s("%");
            lcd_setCursor(15, 1);
            lcd_write((byte)0); // up
            break;
        default:
            break;
        }
        break;
    case 3: //Speed Mode
        switch (sub)
        {
        case 0:
            break;
        case 1:
            lcd_clear();
            lcd_print_s("Speed Mode:");
            lcd_setCursor(12, 0);
            lcd_print_u(speedMode);
            lcd_setCursor(15, 1);
            lcd_write((byte)1); // down arrow
            break;
        case 2:
            lcd_clear();
            lcd_print_s("S To EDIT->");
            lcd_setCursor(0, 1);
            lcd_print_s("Speed Mode:");
            lcd_setCursor(12, 1);
            lcd_print_u(speedMode);
            lcd_setCursor(15, 1);
            lcd_write((byte)0); // up
            break;
        default:
            break;
        }
        break;
    case 4:
        sub = 2;
        break;
    }
}
void updateLevel_3()
{
    switch (menu)
    {
    case 0:
        break;
    case 1:
        lcd_clear();
        lcd_print_s("Batt.Cap:");
        lcd_setCursor(9, 0);
        lcd_print_u(batteryAttachedBMScap);
        break;
    case 2:
        lcd_clear();
        lcd_print_s("Batt.Low:");
        lcd_setCursor(9, 0);
        lcd_print_u(battLowThreshold);
        break;
    case 3:
        lcd_clear();
        lcd_print_s("Speed Mode:");
        lcd_setCursor(12, 0);
        lcd_print_u(speedMode);
        break;
    case 4:
        sub = 2;
        break;
    }
}