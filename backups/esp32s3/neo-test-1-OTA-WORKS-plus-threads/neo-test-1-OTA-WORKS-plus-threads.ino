/*

NeoPixel LEDs

modified on 7 May 2019
by Saeed Hosseini @ Electropeak
https://electropeak.com/learn/

*/

//#include <Adafruit_NeoPixel.h>
#include <sk.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define OTA_ENABLE true
#include "BaseOTA.h"

#define PIN        48
#define NUMPIXELS 2


//Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
sk sk;

TaskHandle_t Task1;

const int xPin = 5;    // X output of the accelerometer
const int yPin = 4;   // Y output of the accelerometer


void NeoBlink(int num, int wait)
{

sk.color(0, 0, 0, 255, 0);
sk.color(0, 0, 0, 255, 0);
sk.color(0, 0, 0, 255, 0);
delay(20);

sk.show();
//delay(wait);
delay(500);
sk.color(0, 255, 0, 0, 0);
sk.color(0, 255, 0, 0, 0);
sk.color(0, 255, 0, 0, 0);
delay(20);

sk.show();
delay(500);
}

void Task1code( void * parameter) {
  for(;;) {
    OTAloopHandler();
    delay(500);
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  
xTaskCreatePinnedToCore(
      Task1code, /* Function to implement the task */
      "Task1", /* Name of the task */
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      5,  /* Priority of the task */
      &Task1,  /* Task handle. */
      0); /* Core where the task should run */
      
OTAwifi();
OTAinit();  // setup OTA handlers and show IP
//sk.channel(1);
sk.begin(48, 1);
sk.setbrightness(50);
}

void loop()
{
  // variables to read the pulse widths:
  int pulseX, pulseY;
  // variables to contain the resulting accelerations
  int accelerationX, accelerationY;

  // read pulse from x- and y-axes:
  pulseX = pulseIn(xPin, HIGH);
  pulseY = pulseIn(yPin, HIGH);

  // convert the pulse width into acceleration
  // accelerationX and accelerationY are in milli-g's:
  // Earth's gravity is 1000 milli-g's, or 1 g.
  accelerationX = ((pulseX / 10) - 500) * 8;
  accelerationY = ((pulseY / 10) - 500) * 8;

  // print the acceleration
  Serial.print(accelerationX);
  // print a tab character:
  Serial.print("\t");
  Serial.print(accelerationY);
  Serial.println();
  int analogValue = analogRead(6);

  Serial.print("Analog reading: ");
  Serial.print(analogValue);   // the raw analog reading

  Serial.println(analogValue);

//  delay(100);
  
NeoBlink(7, 500);
//delay(50);



}
