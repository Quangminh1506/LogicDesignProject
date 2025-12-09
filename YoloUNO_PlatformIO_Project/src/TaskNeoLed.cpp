// TaskNeoLed.cpp
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "global.h"

void TaskNeoLed(void *pvParameters) {
  Adafruit_NeoPixel pixels3(4, NEOPIXEL_GPIO, NEO_GRB + NEO_KHZ800);
  pixels3.begin();
  pixels3.clear();
  pixels3.show();

  while (1) {
    // Wait for new value (TaskTempHumid xSemaphoreGive(humidSem))
    if (xSemaphoreTake(humidSem, portMAX_DELAY) == pdTRUE) {
      float localHumid = gCurrentHumid;

      uint8_t r = 0, g = 0, b = 0;

      if (localHumid < 60.0f) {
        // Low humid -> blue
        r = 0;   g = 0;   b = 255;
      } else if (localHumid < 80.0f) {
        // Moderate humid -> green
        r = 0;   g = 255; b = 0;
      } else {
        // High humid -> red
        r = 255; g = 0;   b = 0;
      }

      pixels3.clear();
      // set all 4 LEDs
      uint32_t color = pixels3.Color(r, g, b);
      for (int i = 0; i < 4; i++) {
        pixels3.setPixelColor(i, color);
      }
      pixels3.show();
    }
  }
}
