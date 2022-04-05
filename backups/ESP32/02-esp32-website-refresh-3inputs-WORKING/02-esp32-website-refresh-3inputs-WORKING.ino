/*
 * ESP32 AJAX Demo
 * Updates and Gets data from webpage without page refresh
 * https://circuits4you.com
 */
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

#include "index.h"  //Web page header file

WebServer server(80);

//Enter your SSID and PASSWORD
const char* ssid = "SpectrumSetup-D5";
const char* password = "energymonth426";

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}
 
void handleADC() {
 int a = analogRead(A0);
 float vout = (a*3.29)/ 4095.0;
 String adcValue = String(vout);
 server.send(200, "text/plane", adcValue); //Send ADC value only to client ajax request
}

void handleADC2() {
  int b = analogRead(A1);
  float vout2 = (b*3.29)/ 4095.0;
  String adcValue2 = String(vout2);
 server.send(200, "text/plane", adcValue2); //Send ADC value only to client ajax request
}

void handleADC3() {
 int c = analogRead(A2);
 float vout3 = (c*3.29)/ 4095.0;
 String adcValue3 = String(vout3);
 server.send(200, "text/plane", adcValue3); //Send ADC value only to client ajax request
}

void handleADC4() {
 int d = analogRead(A8);
 float vout4 = (d*3.29)/ 4095.0;
 String adcValue4 = String(vout4);
 server.send(200, "text/plane", adcValue4); //Send ADC value only to client ajax request
}

void handleADC5() {
  int e = analogRead(A9);
  float vout5 = (e*3.29)/ 4095.0;
  String adcValue5 = String(vout5);
 server.send(200, "text/plane", adcValue5); //Send ADC value only to client ajax request
}

void handleADC6() {
 int f = analogRead(A5);
 float vout6 = (f*3.29)/ 4095.0;
 String adcValue6 = String(vout6);
 server.send(200, "text/plane", adcValue6); //Send ADC value only to client ajax request
}

void handleADC7() {
 int g = analogRead(A6);
 float vout7 = (g*3.29)/ 4095.0;
 String adcValue7 = String(vout7);
 server.send(200, "text/plane", adcValue7); //Send ADC value only to client ajax request
}

void handleADC8() {
  int h = analogRead(A7);
  float vout8 = (h*3.29)/ 4095.0;
  String adcValue8 = String(vout8);
 server.send(200, "text/plane", adcValue8); //Send ADC value only to client ajax request
}

//===============================================================
// Setup
//===============================================================

void setup(void){
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");

/*
//ESP32 As access point
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP(ssid, password);
*/
//ESP32 connects to your wifi -----------------------------------
  WiFi.mode(WIFI_STA); //Connectto your wifi
  WiFi.begin(ssid, password);

  Serial.println("Connecting to ");
  Serial.print(ssid);

  //Wait for WiFi to connect
  while(WiFi.waitForConnectResult() != WL_CONNECTED){      
      Serial.print(".");
    }
    
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
//----------------------------------------------------------------
 
  server.on("/", handleRoot);      //This is display page
  server.on("/readADC", handleADC);//To get update of ADC Value only
  server.on("/readADC2", handleADC2);//To get update of ADC Value only
  server.on("/readADC3", handleADC3);//To get update of ADC Value only
  server.on("/readADC4", handleADC4);//To get update of ADC Value only
  server.on("/readADC5", handleADC5);//To get update of ADC Value only
  server.on("/readADC6", handleADC6);//To get update of ADC Value only
  server.on("/readADC7", handleADC7);//To get update of ADC Value only
  server.on("/readADC8", handleADC8);//To get update of ADC Value only
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void loop(void){
  server.handleClient();
  delay(1);
}
