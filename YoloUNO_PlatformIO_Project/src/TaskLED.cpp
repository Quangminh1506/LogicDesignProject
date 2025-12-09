// TaskLED.cpp
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "global.h"

// LED GPIO6 will blink depends on temperature
//  - T < 29°C        : blink slow (1.5s ON, 1.5s OFF)
//  - 29°C <= T < 31°C : blink moderate (1s ON, 1s OFF)
//  - T >= 28°C        : blink fast (0.5s ON, 0.5s OFF)
//
// After TaskTempHumid read new temp, it will perform xSemaphoreGive(tempSem),
// TaskLEDControl will take xSemaphoreTake(tempSem).

void TaskLEDControl(void *pvParameters) {
  pinMode(LED_GPIO, OUTPUT);
  digitalWrite(LED_GPIO, LOW);

  while (1) {
    if (xSemaphoreTake(tempSem, portMAX_DELAY) == pdTRUE) {
      float localTemp = gCurrentTempC;

      TickType_t onTime;
      TickType_t offTime;

      if (localTemp < 29.0f) {
        onTime  = pdMS_TO_TICKS(1500);
        offTime = pdMS_TO_TICKS(1500);
      } else if (localTemp < 31.0f) {
        onTime  = pdMS_TO_TICKS(800);
        offTime = pdMS_TO_TICKS(800);
      } else {
        onTime  = pdMS_TO_TICKS(300);
        offTime = pdMS_TO_TICKS(300);
      }

      digitalWrite(LED_GPIO, HIGH);
      vTaskDelay(onTime);
      digitalWrite(LED_GPIO, LOW);
      vTaskDelay(offTime);
    }
  }
}
