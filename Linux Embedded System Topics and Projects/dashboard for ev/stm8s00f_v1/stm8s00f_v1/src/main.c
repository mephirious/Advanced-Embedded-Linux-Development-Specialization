#include <Arduino.h>
#include "LiquidCrystal.h"
#include "jk_bms.h"
#include "menu.h"

static uint8_t receivedBuffer[RECEVIED_BUFFER_LENGTH];
static unsigned long previousMillis = 0;

extern LiquidCrystal (lcd,PD3,PD1,PC7,PC6,PC5,PC4);

LiquidCrystal (lcd,PD3,PD1,PC7,PC6,PC5,PC4);

extern byte menuLevel;
//byte menuLevel = 0;

void processBMSdata(uint8_t *data);

void setup()  {
  Serial_begin(115200);
  configure_menu_button();
  lcd_begin(16,2);
  create_char();
  pinMode(PB5,OUTPUT);
} 

void loop()  {
  
  
 checkButton();
  
  if (menuLevel == 0)
  {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= JK_BMS_READ_INTERVAL)
    {
      requestToJkmBms();
      fillBatteryBuffer(receivedBuffer);
      if (strlen(receivedBuffer) == 0)
      {
        lcd_clear();
        lcd_print_s("No BMS");
        //No Menu and no BMS is connected.
      }
      else
      {
        processBMSdata(receivedBuffer);
        memset(receivedBuffer,0,sizeof(receivedBuffer));
        //Display Main Data On the Display.
      }
      previousMillis = currentMillis;
    }
  }

  digitalWrite(PB5,HIGH);
  delay(50);
  digitalWrite(PB5,LOW);
  delay(50); 
  
}

void processBMSdata(uint8_t *data)
{
  uint8_t    RemainingCapacity = 0;
  uint16_t   current = 0;
  uint16_t   voltage = 0;
  voltage = (uint16_t)data[0] << 8 | data[1];
  current = (uint16_t)data[2] << 8 | data[3];
  RemainingCapacity = data[4];
  
  lcd_clear();
  lcd_print_s("V:");
  lcd_print_fd(voltage*0.01,2);
  lcd_print_s("||CP:");
  lcd_print_u(RemainingCapacity);
  lcd_setCursor(0,1);
  lcd_print_s("I:");
  lcd_print_fd(current*0.01,2);


}