// Simple I2C test for ebay 128x64 oled.

#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

#include "BluetoothSerial.h"
#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti wifiMulti;

#define MAX_SRV_CLIENTS 1
const char* ssid = "MySpectrumWiFi72-2G";
const char* password = "greengate335";

WiFiServer server(23);
WiFiClient serverClients[MAX_SRV_CLIENTS];

BluetoothSerial SerialBT;



// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN 16
#define analogInPin 35  

SSD1306AsciiWire oled;

//------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);

  SerialBT.begin("DumpsterDiveMM"); //Bluetooth device name
  Serial.print("The device started, now you can pair it with bluetooth!");
  Wire.begin();
  Wire.setClock(400000L);
  analogReadResolution(12); //12 bits
//  analogSetAttenuation(ADC_0db);  //For all pins
  wifiMulti.addAP(ssid, password);
  
  pinMode(2,OUTPUT);
  digitalWrite(2, HIGH);
#if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
#endif // RST_PIN >= 0
  oled.setFont(Adafruit5x7);
    // true or false determines screen orientation 
  oled.displayRemap(true);
  oled.clear();
  oled.set2X();
  oled.println("Meter");

  Serial.println("Connecting Wifi ");
  for (int loops = 5; loops > 0; loops--) {
    if (wifiMulti.run() == WL_CONNECTED) {
      Serial.println("");
      Serial.print("WiFi connected ");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      break;
   }
    else {
      Serial.println("not connected");
      delay(100);
    }
  }
  if (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("WiFi connect failed");
    delay(1000);
 //   ESP.restart();
  }

  //start UART and the server
  Serial2.begin(115200);
  Serial.print("test serial2");
  server.begin();
  server.setNoDelay(true);

  Serial.print("Ready! Use 'telnet ");
  Serial.print(WiFi.localIP());
  Serial.println(" 23' to connect");
  
}
//------------------------------------------------------------------------------
void loop() {

  uint8_t i;

   int sum = 0;
   int sum1 = 0;
   analogReadResolution(12);
   int analogValue = analogRead(analogInPin);
   analogReadResolution(12);
   int analogValue2 = analogRead(34);
   sum += analogValue;
   sum1 += analogValue2;
//   float ave = (float)sum/30.0;
   float vout = (sum*3.29)/ 4096.0 ;
//   float va = (vout / (10000.0/110000.0));
//   float ave2 = (float)sum1/30.0;
   float vout2 = (sum1*3.29)/ 4096.0 ;
//   float va2 = (vout2 / (10000.0/110000.0));
//   Serial.print("sum");
//   Serial.println(sum);
//   Serial.print("vout");
//   Serial.println(vout);
//   Serial.print("va");
//   Serial.println(va);
   oled.set2X();
   oled.clearField(1, 27, 14);
   oled.setCursor(0, 2);
   oled.print("DC1   ");
   oled.print(vout);
//   oled.print(va);
   
   oled.clearToEOL();
 //  oled.println();
 //  oled.println();
 //  oled.clearField(1, 27, 14);
   oled.setCursor(0, 5);
   oled.print("DC2   ");
   oled.print(vout2);
//   oled.print(va2);

   if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
 
 // delay(20);

   if (wifiMulti.run() == WL_CONNECTED) {
    //check if there are any new clients
    if (server.hasClient()){
      for(i = 0; i < MAX_SRV_CLIENTS; i++){
        //find free/disconnected spot
        if (!serverClients[i] || !serverClients[i].connected()){
          if(serverClients[i]) serverClients[i].stop();
          serverClients[i] = server.available();
          if (!serverClients[i]) Serial.println("available broken");
          Serial.print("New client: ");
          Serial.print(i); Serial.print(' ');
          Serial.println(serverClients[i].remoteIP());
//          break;
        }
      }
      if (i >= MAX_SRV_CLIENTS) {
        //no free/disconnected spot so reject
        server.available().stop();
      }
    }
    //check clients for data
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      if (serverClients[i] && serverClients[i].connected()){
        if(serverClients[i].available()){
          //get data from the telnet client and push it to the UART
          while(serverClients[i].available()) Serial.write(serverClients[i].read());
        }
      }
      else {
        if (serverClients[i]) {
          serverClients[i].stop();
        }
      }
    }
    //check UART for data
    if(Serial.available()){
      size_t len = Serial.available();
      uint8_t sbuf[len];
      Serial.readBytes(sbuf, len);
      //push UART data to all connected telnet clients
      for(i = 0; i < MAX_SRV_CLIENTS; i++){
        if (serverClients[i] && serverClients[i].connected()){
          serverClients[i].write(sbuf, len);
          delay(1);
        }
      }
    }
  }
  else {
    Serial.println("WiFi not connected!");
//    for(i = 0; i < MAX_SRV_CLIENTS; i++) {
//      if (serverClients[i]) serverClients[i].stop();
    }
//    delay(1000);
//  }
}
