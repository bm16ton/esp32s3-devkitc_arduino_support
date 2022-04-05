// Copyright 2020 Christian Nilsson (@nikize)
// Based on public Arduino BasicOTA example

#ifndef EXAMPLES_IRRECVDUMPV3_BASEOTA_H_
#define EXAMPLES_IRRECVDUMPV3_BASEOTA_H_

#ifndef OTA_ENABLE
#define OTA_ENABLE false
#endif  // OTA_ENABLE

#if OTA_ENABLE

#include <WiFi.h>
// #include <ESP8266WiFi.h>
//#include <ESPmDNS.h>
#include <mdns.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

void OTAwifi() {
  // start default wifi (previously saved on the ESP) for OTA
  WiFi.mode(WIFI_STA);
  WiFi.begin();
}

void OTAinit() {
  // See BasicOTA ESP example for source and settings

//  ArduinoOTA
    ArduinoOTA.onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else
        type = "filesystem";

      Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
      Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();
  Serial.println();
  if (WiFi.waitForConnectResult() == WL_CONNECTED) {
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Wifi Connection Failed.");
  }
}

void OTAloopHandler() {
  ArduinoOTA.handle();
}

#else   // OTA_ENABLE
void OTAwifi() {}
void OTAinit() {}
void OTAloopHandler() {}
#endif  // OTA_ENABLE
#endif  // EXAMPLES_IRRECVDUMPV3_BASEOTA_H_
