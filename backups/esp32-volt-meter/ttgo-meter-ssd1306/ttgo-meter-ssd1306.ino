//ESP 32 TTG0 T-DISPLAY Voltmeter
//
//James Steven Dulangon     
//


//#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
//#include <SPI.h>
#include "OLEDDisplayUi.h"
//#include "images.h"
//TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
//#include "SSD1306Wire.h"

  SSD1306  display(0x3c, 4, 15);

OLEDDisplayUi ui     ( &display );

void vaOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(128, 0, String());
}

#define analogInPin 13                

void printBuffer(void) {
  // Initialize the log buffer
  // allocate memory to store 8 lines of text and 30 chars per line.
  display.setLogBuffer(5, 30);



  for (uint8_t i = 0; i < 11; i++) {
    display.clear();
    // Print to the screen
    // Draw it to the internal screen buffer
    display.drawLogBuffer(0, 0);
    // Display it on the screen
    display.display();
    delay(500);
  }
}

OverlayCallback overlays[] = { vaOverlay };
int overlaysCount = 1;
  
void setup()
{
  Serial.begin(115200);
  pinMode(16,OUTPUT);
  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50); 
  digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in high
  display.setLogBuffer(5, 30);
  display.init();
  display.setContrast(255);
//  display.clear();
//  display.println("multimeter!");
//  display.drawLogBuffer(0, 0);
//  display.display();
  delay(10);
//  printBuffer();
  delay(100);
  ui.init();
//  display.setOverlays(overlays, va);
}


void loop()
{   
  
  
  uint8_t num_sample = 30;
  int sum = 0;  

   while (0 < num_sample) 
   {            
      int analogValue = analogRead(analogInPin);
      sum += analogValue;
      num_sample--;                                
      delay(3);
   }
    
   float ave = (float)sum/30.0;
   float vout = (ave*3.29)/ 4096.0 ;
   float va = (vout / (10000.0/110000.0));

//   displayReading(va);
//  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
//  delay(50); 
//  digitalWrite(16, HIGH); 
 // delay(50);
 // display.init();
  display.clear();
  display.display();
  delay(100);
//  display.setTextAlignment(TEXT_ALIGN_LEFT);
//  display.setFont(ArialMT_Plain_10);
//  display.setCursor(0,1);
//  display.drawString(128, 0, "V ");
  display.println(vout);
  display.drawLogBuffer(0, 0);
  display.display();
  printBuffer();
}
