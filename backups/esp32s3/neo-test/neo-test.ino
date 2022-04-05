#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "Arduino.h"

#include "esp32-hal.h"

#define BUILTIN_RGBLED_PIN   48

rmt_data_t led_data[48];

rmt_obj_t* rmt_send = NULL;

void setup() 
{
    Serial.begin(115200);
    
    if ((rmt_send = rmtInit(BUILTIN_RGBLED_PIN, RMT_TX_MODE, RMT_MEM_64)) == NULL)
    {
        Serial.println("init sender failed\n");
    }

    float realTick = rmtSetTick(rmt_send, 100);
    Serial.printf("real tick set to: %fns\n", realTick);

}

int color[] =  { 0x55, 0x11, 0x77 };

void loop() 
{
      color[0] & (1<<(7));
                    led_data[0].level0 = 1;
                    led_data[0].duration0 = 4;
                    led_data[0].level1 = 0;
                    led_data[0].duration1 = 8;
    rmtWrite(rmt_send, led_data, 48);

    delay(100);
}
