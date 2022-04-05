/*
 * ESP32 AJAX Demo
 * Updates and Gets data from webpage without page refresh
 * https://circuits4you.com
 */
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <sk.h>

#define OTA_ENABLE true
#include "BaseOTA.h"

#include "index.h"  //Web page header file
#include "WiFiTerm.h"

sk sk;

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

const uint32_t baud_rates[] = {
  300,
  1200,
  2400,
  4800,
  9600,
  19200,
  38400,
  57600,
  74880,
  115200,
  230400
};
const uint8_t nb_baud_rates = sizeof(baud_rates) / sizeof(uint32_t);

uint8_t  baud_rate_selection = 0;
uint32_t baud_rate = 0;
uint8_t timeout_ms = 1;

//===============================================================
// Setup
//===============================================================

void setup(void){
  sk.begin(48, 1);
  sk.rgbred();
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
    delay(250);
    sk.rgbred();
    delay(250);
    sk.rgbset(255,255,255);
    }
  sk.setbrightness(10);
  delay(20);
  sk.rgbgreen();
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
  term.begin(server);
//term.setAsDefaultWhenUrlNotFound();  
    while (baud_rate_selection == 0)
  {
    term.println("Please select Serial baud rate :");
    for (size_t i=0; i<nb_baud_rates; i++)
    {
      term.print(i+1);
      term.print(". ");
      term.println(baud_rates[i]);
      sk.setbrightness(10);
      delay(250);
      sk.rgbred();
      delay(250);
      sk.setbrightness(10);
      sk.rgbgreen();
    }
    term.print('>');

    while (!term.available())
    {
      sk.setbrightness(10);
      sk.rgbred();
      delay(250);
      sk.setbrightness(10);
      sk.rgbgreen();
      server.handleClient();
      term.handleClient();
      delay(100);
    } 

    int i = term.parseInt();
    term.flush();
    term.println(i);
    if (1 <= i && i <= nb_baud_rates)
    {
      baud_rate_selection = i;
      baud_rate = baud_rates[baud_rate_selection - 1];
      timeout_ms = 1 + 10000 / baud_rate;
    }
    else
    {
      term.println("Invalid selection\n");
    }
  }
  delay(20);
  sk.rgbgreen();
  Serial.begin(baud_rate);
  term.print("Gateway started at ");
  term.print(baud_rate);
  term.println(" bps");
  term.println();
}

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void loop(void){
  OTAloopHandler();
  server.handleClient();
  delay(1);
  term.handleClient();

  //term to Serial
  if (term.available())
  {
    Serial.println(term.readString());
  }

  //Serial to term
  size_t qty = WIFITERM_TX_BUF_SIZE;
  while (Serial.available() && qty--)
  {
    char c = Serial.read();
    term.print(c);
    if (!Serial.available())
    {
      delay(timeout_ms); //wait for next char
    }
  }
}
