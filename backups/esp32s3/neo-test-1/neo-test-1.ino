/*

NeoPixel LEDs

modified on 7 May 2019
by Saeed Hosseini @ Electropeak
https://electropeak.com/learn/

*/

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define OTA_ENABLE true
#include "BaseOTA.h"

#define PIN        48
#define NUMPIXELS 1


Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void NeoBlink(int num, int wait)
{
for (int i = 0; i < num; i++)
{
pixels.setPixelColor(i, 0, 0, 255);
}
pixels.show();
delay(wait);
for (int j = 0; j < num; j++)
{
pixels.setPixelColor(j, 255, 0, 0);
}
pixels.show();
delay(wait);
}

void setup()
{
OTAwifi();
OTAinit();  // setup OTA handlers and show IP

pixels.begin();
pixels.setBrightness(50);
}

void loop()
{
NeoBlink(7, 500);
delay(50);

OTAloopHandler();

}
