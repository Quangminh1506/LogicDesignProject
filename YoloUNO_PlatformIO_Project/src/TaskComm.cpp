// TaskWifi.cpp
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "global.h"
#include <ESP32Servo.h> 

// IP address
IPAddress local_IP(172, 28, 182, 55);
IPAddress gateway(172, 28, 182, 1);
IPAddress subnet(255, 255, 255, 0);

WiFiServer server(8080);

Servo doorServo;

void TaskComm(void *pvParameters) {
  doorServo.setPeriodHertz(50);   
  doorServo.attach(DOOR_SERVO_GPIO, 500, 2400); 

  // closed door (default)
  doorServo.write(0);

  // 2. IP config
  WiFi.config(local_IP, gateway, subnet);

  // 3. connect wifi
  WiFi.begin(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    vTaskDelay(pdMS_TO_TICKS(500));
  }

  // start listening from pi
  server.begin();

  while (1) {
    WiFiClient client = server.available();

    if (client) {
      while (client.connected()) {
        if (client.available()) {
          char c = client.read();

          // control servo
          if (c == '1') {
            // open door (90 degree)
            doorServo.write(90);
            if (xSemaphoreTake(serialMutex, portMAX_DELAY) == pdTRUE) {
              Serial.println("DOOR STATE: OPEN");
              xSemaphoreGive(serialMutex);
            }
          } 
          else if (c == '0') {
            // close door
            doorServo.write(0);
            if (xSemaphoreTake(serialMutex, portMAX_DELAY) == pdTRUE) {
              Serial.println("DOOR STATE: CLOSED");
              xSemaphoreGive(serialMutex);
            }
          }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
      }
      client.stop();
    }
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}
