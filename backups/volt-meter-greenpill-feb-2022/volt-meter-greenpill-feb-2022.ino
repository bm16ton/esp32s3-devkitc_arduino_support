/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogReadSerial
*/

#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN -1

SSD1306AsciiWire oled;

uint8_t col[4]; // Columns for ADC values.
uint8_t rows;   // Rows per line.
// the setup routine runs once when you press reset:
void setup() 
{
  Wire.begin();
  Wire.setClock(400000L);
  pinMode(PC13, OUTPUT);
  digitalWrite(PC13, HIGH);
  pinMode(PC14, INPUT);
#if RST_PIN >= 0
  oled.begin(&Adafruit128x32, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
  oled.begin(&Adafruit128x32, I2C_ADDRESS);
#endif // RST_PIN >= 0

  oled.setFont(Adafruit5x7);

  uint32_t m = micros();
  oled.clear();
 // oled.set2X();
//  oled.println("Starting");
  // initialize serial communication at 9600 bits per second:
  SerialUSB.begin(115200);
  col[0] = oled.fieldWidth(strlen("A0=9999"));
  col[1] = oled.fieldWidth(strlen("A0=9999"));
  rows = oled.fontRows();
  delay(300);  
}

// the loop routine runs over and over again forever:
void loop() {
//  oled.clear(); 
//  for (uint8_t i = 0; i < 6; i++) {   
//  oled.clearField(col[i%2], rows*(i/2), 4);  
//for (uint8_t i = 0; i < 6; i++) {  
   //oled.clearField(col[7], rows*(2), 4); 
  // read the input 4.`on analog pin 0:
  oled.clearField(1, 27, 14);
  oled.setCursor(0, 0);
  int sensorValueA0 = analogRead(A0);
  float inputVoltage0 = (float(sensorValueA0)/1024) * 3.3;
  int sensorValueA1 = analogRead(A1);
  float inputVoltage1 = (float(sensorValueA1)/1024) * 3.3;
  int sensorValueA2 = analogRead(A2);
  float inputVoltage2 = (float(sensorValueA2)/1024) * 3.3;
  int sensorValueA3 = analogRead(A3);
  float inputVoltage3 = (float(sensorValueA3)/1024) * 3.3;
  int sensorValueA4 = analogRead(A4);
  float inputVoltage4 = (float(sensorValueA4)/1024) * 3.3;
  int sensorValueA5 = analogRead(A80);
  float inputVoltage5 = (float(sensorValueA5)/1024) * 3.3;
  // print out the value you read:
//  oled.clearToEOL();
  oled.print("A0=");
  oled.print(inputVoltage0 * 2);
  SerialUSB.print("A0 = ");
  SerialUSB.println(inputVoltage0 * 2);
//  oled.clearToEOL();
  oled.print(" A1=");
  oled.println(inputVoltage1 * 2);
  SerialUSB.print("A1 = ");
  SerialUSB.println(inputVoltage1 * 2);
//  oled.clearToEOL();
  oled.print("A2=");
  oled.print(inputVoltage2 * 2);
  SerialUSB.print("A2 = ");
  SerialUSB.println(inputVoltage2 * 2);

    oled.print(" A3=");
  oled.println(inputVoltage3 * 2);
  SerialUSB.print("A3 = ");
  SerialUSB.println(inputVoltage3 * 2);
//  oled.clearToEOL();
  oled.print("A4=");
  oled.print(inputVoltage4 * 2);
  SerialUSB.print("A4 = ");
  SerialUSB.print(inputVoltage4 * 2);
//  oled.clearToEOL();
  oled.print(" A5=");
  oled.println(inputVoltage5 * 2);
  SerialUSB.print("A5 = ");
  SerialUSB.println(inputVoltage5 * 2);
//  }
  delay(1);        // delay in between reads for stability
}
