#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include "DHT20.h"
#include "LiquidCrystal_I2C.h"
#include <Adafruit_NeoPixel.h>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

// ================== WiFi config ==================
extern const char* WIFI_SSID;
// extern const char* WIFI_PASSWORD;

// ================== GPIO defines ==================
#define LED_GPIO        GPIO_NUM_6
#define DOOR_SERVO_GPIO GPIO_NUM_1
#define WIFI_LED_GPIO   GPIO_NUM_48
#define NEOPIXEL_GPIO   GPIO_NUM_8
#define I2C_SDA_GPIO    GPIO_NUM_11
#define I2C_SCL_GPIO    GPIO_NUM_12

// ================== semaphore handles ==================
extern SemaphoreHandle_t serialMutex; 
extern SemaphoreHandle_t tempSem;      
extern SemaphoreHandle_t humidSem;   

// ================== shared data ==================
extern volatile float gCurrentTempC; 
extern volatile float gCurrentHumid;   

// create global vals
void initGlobals();