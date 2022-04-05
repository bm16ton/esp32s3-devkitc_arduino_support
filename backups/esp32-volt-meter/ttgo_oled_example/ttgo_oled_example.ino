#include "OLEDDisplayUi.h"

#include <Wire.h>        // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy: #include "SSD1306.h"

// TTGO LoRa32-OLED V1
//
// #define OLED_SDA 4
// #define OLED_SCL 15
// #define OLED_RST 16

// TTGO LoRa32-OLED v2.0 and V2.1
// 
// #define OLED_SDA 21
// #define OLED_SCL 22
// #define OLED_RST NOT_A_PIN

SSD1306Wire display(0x3c, OLED_SDA, OLED_SCL);

OLEDDisplayUi ui     ( &display );

void setup()
{
  Serial.begin(9600);
  delay(100);
  Serial.println("hello world");

  // reset the OLED:
  if (OLED_RST != NOT_A_PIN)
  {
    pinMode(OLED_RST, OUTPUT);
    digitalWrite(OLED_RST, LOW); // set GPIO16 low to reset OLED
    delay(50);
    digitalWrite(OLED_RST, HIGH); // while OLED is running, must set GPIO16 in high
  }

  display.setLogBuffer(5, 30);
  display.init();
  display.flipScreenVertically();
  display.clear();
  display.setFont(ArialMT_Plain_10);
//  display.setContrast(255);
//  display.clear();
//  display.println("multimeter!");
//  display.drawLogBuffer(0, 0);
//  display.display();
//  delay(10);
//  printBuffer();
//  delay(100);
  ui.init();
   display.drawString(0, 0, "Hello world");
    display.display();
//  display.setOverlays(overlays, va);
/*
  display.init();
  display.flipScreenVertically();

  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Hello world");
  display.display();
  */
}

void loop()
{
}
