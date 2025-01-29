#include "Arduino.h"
#include "jk_bms.h"
#include "menu.h"
#include "LiquidCrystal.h"
#include "EEPROM.h"

#define BUZZER_PIN PA1
#define BUZZER_PIN_BEEP_INTERVAL 800

static unsigned long previousMillis = 0;
static unsigned long beePpreviousMillis = 0;
static uint8_t receivedBuffer[RECEVIED_BUFFER_LENGTH];

LiquidCrystal (lcd,PD3,PD2,PD1,PC7,PC6,PC5,PC4);

uint8_t RemainingCapacity = 100; //Making it global so that it can access by Buzzer.
uint8_t buzzerState = LOW;

extern uint8_t main_dis;
extern int addr;
extern uint8_t km_range;

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
  lcd_print_s("Firmware V1.00");
  delay(3000);
  lcd_clear();
} 

void loop()  {
  // receivedBuffer[0] = 0x19;
  // receivedBuffer[1] = 0xfb;
  // receivedBuffer[2] = 0x3A;
  // receivedBuffer[3] = 0xBB;
  // receivedBuffer[4] = 0x64;
  buttonOperation(checkButton());
  if (main_dis == 0)
  {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= JK_BMS_READ_INTERVAL)
    {
      requestToJkmBms();
      if (!fillBatteryBuffer(receivedBuffer))
      {
        lcd_clear();
        lcd_print_s("No JK-BMS");
        //No Menu and no BMS is connected.
      }
      else
      {
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

void processBMSdata(uint8_t *data)
{ 
  float   current = 0;
  float   voltage = 0;
  float   power   = 0;

  /*Temporary for display*/
  uint16_t tpvoltage = (uint16_t)data[0] << 8 | data[1]; //volatge two combine byte 
  uint16_t tpcurrent = (uint16_t)data[2] << 8 | data[3]; //current two combine byte
  RemainingCapacity  = data[4];                           //Capacity one byte

  voltage = tpvoltage * 0.01f;

  if ((tpcurrent & 0x8000) == 0x8000)
  {
    current = (float)(tpcurrent & 0x7FFF);
  }else{current = (float)(tpcurrent & 0x7FFF) * -1;}

  current = current * 0.01f;
  power   =  voltage * current;

  /*Print Remaining Capacity*/
  lcd_clear();
  lcd_print_u(RemainingCapacity);
  lcd_print_c('%');

  /*Calculate the Remaining Range in Kms*/
  lcd_setCursor(5,0);
  lcd_print_u(RemainingCapacity * ((float)km_range/100.0f));
  lcd_print_s("KM");

  lcd_setCursor(10,0);
  lcd_print_i(current);
  lcd_print_c('A');

  /*Print Voltage on the lcd*/
  lcd_setCursor(0,1);
  lcd_print_u(voltage);
  lcd_print_c('V');

  lcd_setCursor(10,1);
  lcd_print_i(power);
  lcd_print_c('W');

  // /*Print Current On the LCD*/
  // if (current > 0.0f)
  // {
  //   //Back Emf.
  //   lcd_setCursor(11,0);
  //   lcd_print_c('+');
  //   lcd_print_u(current);
  //   lcd_print_c('A');
    
  // }
  // else if (current < 0.0)
  // {
  //   lcd_setCursor(11,0);
  //   lcd_print_c('-');
  //   lcd_print_i(current);
  //   lcd_print_c('A');
  // }
  // else
  // {
  //   lcd_setCursor(11,0);
  //   lcd_print_i(current);
  //   lcd_print_c('A');
  // }
  // /*Print Voltage on the lcd*/
  // lcd_setCursor(0,1);
  // lcd_print_u(voltage);
  // lcd_print_c('V');

  /*Print Power on the lcd*/
  // lcd_setCursor(12,1);
  // lcd_print_i(power);
  // lcd_print_c('W');

 }

// void processBMSdata(uint8_t *data)
// { 
//   float   current = 0;
//   float   voltage = 0;
//   float   power   = 0;
//   uint8_t rangeRemaining= 0;

//   voltage = ((uint16_t)data[0] << 8 | data[1])*0.01f;    //volatge two combine byte 
//   current = ((int16_t)data[2]  << 8 | data[3])*0.01f;    //current two combine byte

//   /*Temporary for display*/
//   uint16_t tpvoltage = (uint16_t)data[0] << 8 | data[1]; //volatge two combine byte 
//   uint16_t tpcurrent = (uint16_t)data[2] << 8 | data[3]; //current two combine byte 

//   if (current < 0.0f){current = 0.0; tpcurrent = 0;}
  
//   RemainingCapacity = data[4];                           //Capacity one byte

//   rangeRemaining = RemainingCapacity * 0.6;
//   power = (voltage) * (current);

//   lcd_clear();
//   lcd_print_u(RemainingCapacity);
//   lcd_print_c('%');

//   lcd_setCursor(5,0);
//   lcd_print_u(rangeRemaining);
//   lcd_print_s("KM");

//   if (current < 100.00)
//   {
//     lcd_setCursor(11,0);
//     lcd_print_u(tpcurrent/100);
//     lcd_print_c('.');
//     lcd_print_u(tpcurrent%100);
//     lcd_print_c('A');
//   }
//   else
//   {
//     lcd_setCursor(11,0);
//     lcd_print_u(tpcurrent/100);
//     lcd_print_c('A');
//   }

//   lcd_setCursor(0,1);
//   lcd_print_u(tpvoltage/100);
//   lcd_print_c('.');
//   lcd_print_u(tpvoltage%100);
//   lcd_print_c('V');

//   lcd_setCursor(10,1);
//   lcd_print_u(power);
//   lcd_print_c('W');

//  }

  //  float get_current_(const uint16_t value, const uint8_t protocol_version) {
  //   float current = 0.0f;
  //   if (protocol_version == 0x01) {
  //     if ((value & 0x8000) == 0x8000) {
  //       current = (float) (value & 0x7FFF);
  //     } else {
  //       current = (float) (value & 0x7FFF) * -1;
  //     }
  //   }

  //   return current;
  // };