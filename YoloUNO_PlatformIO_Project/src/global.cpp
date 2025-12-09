// global.cpp
#include "global.h"

const char* WIFI_SSID = "RD-SEAI_2.4G";

SemaphoreHandle_t serialMutex = NULL;
SemaphoreHandle_t tempSem = NULL;
SemaphoreHandle_t humidSem = NULL;

volatile float gCurrentTempC  = 0;
volatile float gCurrentHumid  = 0;

void initGlobals() {
  Serial.begin(115200); 

  // Mutex for serial
  serialMutex = xSemaphoreCreateMutex();

  // Counting semaphore for temp and humid
  tempSem  = xSemaphoreCreateCounting(10, 0);
  humidSem = xSemaphoreCreateCounting(10, 0);

  if (serialMutex == NULL || tempSem == NULL || humidSem == NULL) {
    while (1) {
      Serial.println("Failed to create semaphores");
      vTaskDelay(1000);
    }
  }
}
