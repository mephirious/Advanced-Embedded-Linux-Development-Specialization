#include "Arduino.h"
//#include "jk_bms.h"
#include <LiquidCrystal.h>
#include "lcd.h"
#include "MenuSimple.h"


extern LiquidCrystal(lcd, PD3, PD1, PC7, PC6, PC5, PC4);

extern byte menuLevel;
extern char buttonPressed;


uint8_t ledState = LOW;


void setup()  {

Serial_begin(115200);
  configureLcdPins();
  lcdInitialization();
  ButtonConfigrature();
  lcd_clear();
  lcd_begin(16,2);
  pinMode(PB5,OUTPUT);

} 

void loop()  {
  

 ButtonProcess();



  digitalWrite(PD5, HIGH);
  
  digitalWrite(PD5, LOW);

  
}

