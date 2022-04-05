#include <Wire.h>

#include <SPI.h>              // include libraries
#include <LoRa.h>

#define CARDKB_ADDR 0x5F


const int csPin = 18;          // LoRa radio chip select
const int resetPin = 14;       // LoRa radio reset
const int irqPin = 26;         // change for your board; must be a hardware interrupt pin

byte msgCount = 0; 
byte localAddress = 0xBB;     // address of this device
byte destination = 0xFF;      // destination to send to
long lastSendTime = 0;        // last send time
int interval = 2000;          // interval between sends

void setup()
{
  Serial.begin(115200);

  Wire.begin();
//  Wire.setClock(400000L);
  delay(1000);

LoRa.setPins(csPin, resetPin, irqPin);// set CS, reset, IRQ pin

  if (!LoRa.begin(915E6)) {             // initialize ratio at 915 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }

  Serial.println("LoRa init succeeded.");
//  oled.print("LoRa");
}
void loop()
{
  Wire.requestFrom(CARDKB_ADDR, 1);
  while(Wire.available())
  {
//    Serial.println("in while wire loop");
    char c = Wire.read(); // receive a byte as characterif
    byte incsize = c;
 //   Serial.println(c);

    LoRa.beginPacket(); 
    LoRa.write(destination);
    LoRa.write(localAddress);
    LoRa.write(msgCount); 
    LoRa.write(3);
    
    if (c != 0)
    {
//      Serial.println(c);
    if (c == 0xFFFFFFAF) {
      LoRa.println(0xDA);
      LoRa.endPacket();
      msgCount++;
      return ;
  }
    if (c == 0xFFFFFFB0) {
      LoRa.print(0xD9);
      LoRa.endPacket();
      msgCount++;
      return ;
  }
    if (c == 0xFFFFFFB2) {      //right arrow
      LoRa.print(0xD7);
       LoRa.endPacket();
      msgCount++;
      return ;
  }
    if (c == 0xFFFFFFB1) {      //left arrow
      LoRa.print(0xD8);
      LoRa.endPacket();
      msgCount++;
      return ;
  }
   if (c == 0x1B) {
      LoRa.print(0x1B);
      LoRa.endPacket();
      msgCount++;
      return ;
  }
    if (c == 0x0D) {
      LoRa.print(0xB0);
      LoRa.endPacket();
      msgCount++;
      return ;
  }
//      Serial.println(c, HEX);
      Serial.println(char(c));
      LoRa.println(c);
      LoRa.endPacket();
      msgCount++;

    }
  }
}
