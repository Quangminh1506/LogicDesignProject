// TaskTempHumid.cpp
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "global.h"

void TaskTempHumid(void *pvParameters) {
  DHT20 dht20;
  LiquidCrystal_I2C lcd(33, 16, 2); 

  Wire.begin(I2C_SDA_GPIO, I2C_SCL_GPIO);

  dht20.begin();
  lcd.begin();
  lcd.backlight();

  while (1) {
    dht20.read();
    double temp  = dht20.getTemperature();
    double humid = dht20.getHumidity();
    gCurrentTempC = (float)temp;
    gCurrentHumid = (float)humid;

    // display all LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temp);
    lcd.print(" C");

    lcd.setCursor(0, 1);
    lcd.print("Humid: ");
    lcd.print(humid);
    lcd.print(" %");

    xSemaphoreGive(tempSem);   
    xSemaphoreGive(humidSem); 

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
