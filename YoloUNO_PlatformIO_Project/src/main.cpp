// main.cpp
#include <Arduino.h>
#include "global.h"

#include "TaskLED.h"
#include "TaskNeoLed.h"
#include "TaskTempHumid.h"
#include "TaskComm.h" 

void setup() {
  initGlobals();

  Serial.println("System Initializing...");

  xTaskCreate(TaskComm, "WifiServer", 4096, NULL, 1, NULL);

  xTaskCreate(TaskLEDControl, "LED Control", 2048, NULL, 2, NULL);
  xTaskCreate(TaskNeoLed,     "NeoPixel",    4096, NULL, 2, NULL);
  xTaskCreate(TaskTempHumid,  "TempHumid",   4096, NULL, 2, NULL);
}

void loop() {
  // FreeRTOS task-based
}