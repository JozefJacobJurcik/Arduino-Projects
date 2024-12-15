#include <Arduino.h>
#include <Wire.h>
#include<U8g2lib.h>
#include <Adafruit_MAX31865.h>

#define clk_pin 19
#define sdo_pin 35
#define sdi_pin 32
#define cs_pin 14  


U8G2_SSD1306_128X64_NONAME_F_SW_I2C
u8g2(U8G2_R0,22,21,U8X8_PIN_NONE);

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31865 thermo = Adafruit_MAX31865(cs_pin, sdi_pin, sdo_pin, clk_pin);
// use hardware SPI, just pass in the CS pin

// The value of the Rref resistor. Use 430.0 for PT100 and 4300.0 for PT1000
#define RREF      430.0
// The 'nominal' 0-degrees-C resistance of the sensor
// 100.0 for PT100, 1000.0 for PT1000
#define RNOMINAL  100.0

float temp =0;

void setup(void){
  Serial.begin(115200);
  u8g2.begin();
  thermo.begin(MAX31865_3WIRE);
}

void loop(void){


  
  
  uint16_t rtd = thermo.readRTD();

  Serial.print("RTD value: "); Serial.println(rtd);
  float ratio = rtd;
  ratio /= 32768;
  Serial.print("Ratio = "); Serial.println(ratio,8);
  Serial.print("Resistance = "); Serial.println(RREF*ratio,8);
  temp = thermo.temperature(RNOMINAL, RREF);
  Serial.print("Temperature = "); Serial.println(temp);
  
  delay(1000);
  
  
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_7x14B_tr);
  u8g2.setCursor(50,50);
  u8g2.print(temp,2);
  //u8g2.drawStr(30,25,"nieco");
  //u8g2.drawCircle(20, 15, 10, U8G2_DRAW_ALL);
  
  u8g2.sendBuffer();
  delay(1000);
  u8g2.clearBuffer();
  u8g2.drawCircle(20, 15, 10, U8G2_DRAW_ALL);
  u8g2.sendBuffer();
  delay(1000);
  
}

