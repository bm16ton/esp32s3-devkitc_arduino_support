/*
  LoRa Duplex communication

  Sends a message every half second, and polls continually
  for new incoming messages. Implements a one-byte addressing scheme,
  with 0xFF as the broadcast address.

  Uses readString() from Stream class to read payload. The Stream class'
  timeout may affect other functuons, like the radio's callback. For an

  created 28 April 2017
  by Tom Igoe
*/
#include <SPI.h>              // include libraries
#include <LoRa.h>
#include <Wire.h>

#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN 16

SSD1306AsciiWire oled;

const int csPin = 18;          // LoRa radio chip select
const int resetPin = 14;       // LoRa radio reset
const int irqPin = 26;         // change for your board; must be a hardware interrupt pin

char serialBuffer[31] = "";
char SendPayload[31] = "";
char RecvPayload[31] = "";

String outgoing;              // outgoing message

byte msgCount = 0;            // count of outgoing messages
byte localAddress = 0xBB;     // address of this device
byte destination = 0xFF;      // destination to send to
long lastSendTime = 0;        // last send time
int interval = 2000;          // interval between sends


boolean stringComplete = false;  // whether the string is complete
static int dataBufferIndex = 0;
boolean stringOverflow = false;
char charOverflow = 0;

void setup() {
  Serial.begin(115200);                   // initialize serial
  pinMode(2,OUTPUT);
  delay(100);
  Wire.begin();
  Wire.setClock(400000L);
//  while (!Serial);
  delay(1000);
  pinMode(16,OUTPUT);
//  digitalWrite(16, LOW);
//  delay(50);
//  digitalWrite(16, HIGH);
#if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
  oled.begin(&Adafruit128x32, I2C_ADDRESS);
#endif // RST_PIN >= 0
  delay(100);
  oled.setFont(System5x7);
    // true or false determines screen orientation 
  oled.displayRemap(true);
  oled.clear();
  oled.set2X();
  delay(100);
  oled.print("LoRa Dup");
  delay(300);
  Serial.println("LoRa Duplex");

  // override the default CS, reset, and IRQ pins (optional)
  LoRa.setPins(csPin, resetPin, irqPin);// set CS, reset, IRQ pin

  if (!LoRa.begin(915E6)) {             // initialize ratio at 915 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }

  Serial.println("LoRa init succeeded.");
//  oled.print("LoRa");
}

void loop() {
  serialEvent();
    if (stringComplete) { 
        strcat(SendPayload,serialBuffer); 
        String dataout = String(SendPayload);       
        sendMessage(dataout);
        stringComplete = false;
        SendPayload[0] = 0;
//        memset(dataout, 0, sizeof(dataout));
        dataout.clear();
    }
//  if (!stringComplete) { 
//    serialBuffer[0] = 0;
//  }
  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());
}

void serialEvent() {
  while (Serial.available() > 0 ) {
      char incomingByte = Serial.read();
      
      if (stringOverflow) {
         serialBuffer[dataBufferIndex++] = charOverflow;  // Place saved overflow byte into buffer
         serialBuffer[dataBufferIndex++] = incomingByte;  // saved next byte into next buffer
         stringOverflow = false;                          // turn overflow flag off
      } else if (dataBufferIndex > 31) {
         stringComplete = true;        // Send this buffer out to radio
         stringOverflow = true;        // trigger the overflow flag
         charOverflow = incomingByte;  // Saved the overflow byte for next loop
         dataBufferIndex = 0;          // reset the bufferindex
         break; 
      } 
      else if(incomingByte=='\n'){
          serialBuffer[dataBufferIndex] = 0; 
          stringComplete = true;
      } else {
          serialBuffer[dataBufferIndex++] = incomingByte;
          serialBuffer[dataBufferIndex] = 0; 
      }          
  } // end while()
} // end serialEvent()

void sendMessage(String outgoing) {
  Serial.print("sending pkt=");
//  oled.clearField(1, 47, 24);
//  oled.clearField(0, 47, 24);
//  oled.clearField(, 47, 24);
  oled.setCursor(0, 0);
  oled.print("              ");
  oled.setCursor(0, 0);
  oled.print("TX=" );
  oled.print(outgoing);
  Serial.println(outgoing);
  digitalWrite(2, HIGH);
  LoRa.beginPacket();                   // start packet
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
//  outgoing = "";
  delay(100);
//  serialBuffer[1] = 0;
//   serialBuffer[dataBufferIndex] = 0;
  dataBufferIndex = 0; 
   digitalWrite(2, LOW);
  outgoing.clear();
  outgoing[0] = 0;
//  serialBuffer[0] = 0;
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length

  String incoming = "";

  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  if (incomingLength != incoming.length()) {   // check length for error
    Serial.println("error: message length does not match length");
    return;                             // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xFF) {
    Serial.println("This message is not for me.");
    return;                             // skip rest of function
  }

  // if message is for this device, or broadcast, print details:
  Serial.println("Received from: 0x" + String(sender, HEX));
  Serial.println("Sent to: 0x" + String(recipient, HEX));
  Serial.println("Message ID: " + String(incomingMsgId));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + incoming);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Serial.println();
}
