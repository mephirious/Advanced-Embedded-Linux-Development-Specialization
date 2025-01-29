#include "Arduino.h"
#include "jk_bms.h"
#include "menu.h"
#include "LiquidCrystal.h"
#include "EEPROM.h"

#define BUZZER_PIN PA2

#define BUZZER_PIN_BEEP_INTERVAL 800
#define TRIES_TO_READ_BMS 3
static unsigned long previousMillis = 0;
static unsigned long beePpreviousMillis = 0;
static uint8_t receivedBuffer[RECEVIED_BUFFER_LENGTH];

LiquidCrystal (lcd,PD3,PD2,PD1,PC7,PC6,PC5,PC4);

uint8_t RemainingCapacity = 100; //Making it global so that it can access by Buzzer.
uint8_t buzzerState = LOW;

extern uint8_t main_dis;
extern int addr;
extern uint8_t km_range;

uint8_t triesToRead = 0;


void processBMSdata(uint8_t *data);

byte upDownsymbol[8] = {
	0b00100,
	0b01110,
	0b11111,
	0b00000,
	0b00000,
	0b11111,
	0b01110,
	0b00100
};

byte enterSymbol[8] = {
	0b11100,
	0b00100,
	0b00100,
	0b00100,
	0b00100,
	0b10101,
	0b01110,
	0b00100
};

void setup()  {
  Serial_begin(115200);
  pinMode(BUZZER_PIN,OUTPUT);
  configure_menu_button();
  km_range = EEPROM_read(0);
  lcd_begin(16,2);
  lcd_createChar(0, upDownsymbol);
  lcd_createChar(1, enterSymbol);
  lcd_print_s("   Chip Soul");
  lcd_setCursor(0,1);
  lcd_print_s("   Technology");
  delay(3000);
  lcd_clear();
  lcd_print_s("Firmware V1.03");
  delay(3000);
  lcd_clear();
} 

void loop()  {
  buttonOperation(checkButton());
  if (main_dis == 0)
  {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= JK_BMS_READ_INTERVAL)
    {
      requestToJkmBms();
      if (!fillBatteryBuffer(receivedBuffer))
      {
        triesToRead++;
        if (triesToRead >= TRIES_TO_READ_BMS)
        {
          lcd_clear();
          lcd_print_s("No JK-BMS");
          triesToRead = TRIES_TO_READ_BMS;
        }
        //No Menu and no BMS is connected.
      }
      else
      {
        triesToRead = 0;
        processBMSdata(receivedBuffer);
      }
      previousMillis = currentMillis;
    }
    if (currentMillis - beePpreviousMillis >= BUZZER_PIN_BEEP_INTERVAL)
    {
      if (RemainingCapacity <=20)
      {
        if (buzzerState == LOW){buzzerState = HIGH;}
        else{buzzerState = LOW;}
        digitalWrite(BUZZER_PIN,buzzerState);
      }else
      {
        digitalWrite(BUZZER_PIN,LOW);
      }
      beePpreviousMillis = currentMillis;
    }
    
  }
}

void processBMSdata(uint8_t *data){ 
  float   current = 0.0f;
  float   voltage = 0.0f;
  float   power   = 0.0f;
  /*Temporary for display*/
  uint16_t tpvoltage = (uint16_t)data[0] << 8 | data[1]; //volatge two combine byte 
  uint16_t tpcurrent = (uint16_t)data[2] << 8 | data[3]; //current two combine byte
  RemainingCapacity  = data[4];                          //capacity one byte
  /*Voltage calculation in float*/
  voltage = tpvoltage * 0.01f;
  /*This is to handle the current direction leaving or entering the battery.*/
  if ((tpcurrent & 0x8000) == 0x8000)
  {
	  current = (float) (tpcurrent & 0x7FFF);
  } 
  else
  {
	  current = (float) (tpcurrent & 0x7FFF) * -1;
  }
  /*Current calculation in float form*/
  current =  current * 0.01f;
  /*Power calculation in float form*/
  power   =  voltage * current;

  /*Print remaining capacity*/
  lcd_clear();
  lcd_print_u(RemainingCapacity);
  lcd_print_c('%');

  /*Calculate the remaining range in Kms*/
  lcd_setCursor(5,0);
  lcd_print_u((RemainingCapacity * ((float)km_range/100.0f)));
  lcd_print_s("km");

  /*Print current On the LCD*/
  lcd_setCursor(10,0);
  lcd_print_i(current);
  lcd_print_c('A');
  
  /*Print voltage on the lcd*/
  lcd_setCursor(0,1);
  lcd_print_u(voltage);
  lcd_print_c('.');
  lcd_print_u(tpvoltage%10);
  lcd_print_c('V');

  /*Print power on the lcd*/
  lcd_setCursor(10,1);
  lcd_print_i(power);
  lcd_print_c('W');
}
