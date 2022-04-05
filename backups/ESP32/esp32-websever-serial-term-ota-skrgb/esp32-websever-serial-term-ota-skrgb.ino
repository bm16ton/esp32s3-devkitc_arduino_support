//Gateway Serial <-> WiFiTerm
//with embedded baudrate selector
#include <sk.h>

#define OTA_ENABLE true
#include "BaseOTA.h"

#include <WebServer.h>
const char* ssid = "SpectrumSetup-D5";     //adjust to your own values
const char* password = "energymonth426"; //adjust to your own values

#include "WiFiTerm.h"

sk sk;

WebServer server(80);

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

void setup()
{
//  pinMode(LED_BUILTIN, OUTPUT);
  sk.begin(48, 1);
  sk.rgbred();
  delay(190);
  WiFi.begin(ssid, password);
  OTAinit();  // setup OTA handlers and show IP
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(250);
    sk.rgbred();
    delay(250);
    sk.rgbset(255,255,255);
  }
  sk.setbrightness(10);
  delay(20);
  sk.rgbgreen();
  server.begin();
  term.begin(server);
  term.setAsDefaultWhenUrlNotFound();
  
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
//  OTAwifi();

}

void loop()
{
  OTAloopHandler();
  server.handleClient();
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
