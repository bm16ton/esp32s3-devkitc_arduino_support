// Simple I2C test for ebay 128x64 oled.

#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

#define analogInPin A0  

SSD1306AsciiWire oled;
//------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
//  SerialBT.begin("DumpsterDiveMM"); //Bluetooth device name
  Serial.print("The device started, now you can pair it with bluetooth!");
  Wire.begin();
  Wire.setClock(400000L);

  pinMode(PC13,OUTPUT);
  digitalWrite(PC13, LOW);

  oled.begin(&Adafruit128x64, I2C_ADDRESS);


  oled.setFont(Adafruit5x7);
    // true or false determines screen orientation 
  oled.displayRemap(true);
  oled.clear();
//  oled.set2X();
//  oled.println("Meter");
}
//------------------------------------------------------------------------------
void loop() {


   int sum = 0;
   int sum1 = 0;
   int analogValue = analogRead(analogInPin);
   int analogValue2 = analogRead(A1);
   sum += analogValue;
   sum1 += analogValue2;
   float ave = (float)sum/30.0;
   float vout = (ave*3.29)/ 4096.0 ;
   float va = (vout / (10000.0/110000.0)*11*2);
   float ave2 = (float)sum1/30.0;
   float vout2 = (ave2*3.29)/ 4096.0 ;
   float va2 = (vout2 / (10000.0/110000.0));
   
   oled.set2X();
   oled.clearField(1, 27, 14);
   oled.setCursor(0, 2);
   oled.print("DC1   ");
   oled.print(va);
   oled.clearToEOL();
 //  oled.println();
 //  oled.println();
 //  oled.clearField(1, 27, 14);
   oled.setCursor(0, 5);
   oled.print("DC2   ");
   oled.print(va2);

 // delay(20);
}
