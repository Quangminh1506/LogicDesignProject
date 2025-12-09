# IoT Environmental monitoring and face recognition access control

A hybrid IoT system that integrates real time environmental monitoring using ESP32 S3 with secure face recognition access control using Edge AI on Raspberry Pi 5.

---

## Table of contents
1. Introduction
2. Key features
3. System architecture
4. Hardware pinout
5. Tech stack
6. Installation and setup
7. Usage

---

## Introduction

This project focuses on combining environmental sensing with AI based offline security. The system uses edge computing to perform all tasks without cloud dependence.

1. Controller ESP32 S3: Handles DHT20 sensor readings, LCD display, LED alerts, and servo motor control using FreeRTOS tasks.

2. Brain Raspberry Pi 5: Runs a YOLOv11 classification model to recognize authorized users Minh and Tai and sends unlock commands via TCP socket.

---

## Key features

### Environmental monitoring
- Reads temperature and humidity from DHT20 every second  
- LED alerts based on temperature thresholds  
- NeoPixel ring color changes based on humidity  
- LCD 16x2 displays real time values  

### AI access control
- Custom trained YOLOv11n classifier for face recognition  
- Voting filter requires 5 positive detections out of 10 frames  
- Auto reconnect mechanism for socket communication  

---

## System architecture

The system uses a TCP IP client server model.

Server ESP32 S3  
Static IP 172.28.182.55, port 8080

Client Raspberry Pi 5  
Sends unlock signals through python TCP script

Architecture flow  
Raspberry Pi 5 -> TCP socket -> ESP32 S3  
ESP32 S3 -> I2C -> DHT20 and LCD  
ESP32 S3 -> PWM -> Servo  
ESP32 S3 -> GPIO -> LEDs and NeoPixel

---

## Hardware Pinout

| Component       | Function          | ESP32 S3 GPIO      | Note                 |
|-----------------|-------------------|---------------------|----------------------|
| DHT20           | Temp Humid sensor | SDA 11, SCL 12      | I2C bus              |
| LCD 16x2        | Display           | SDA 11, SCL 12      | I2C address 0x27     |
| Servo motor     | Door lock         | GPIO 1              | PWM 50 Hz            |
| Status LED      | Temp warning      | GPIO 6              | Active high          |
| NeoPixel ring   | Humid warning     | GPIO 8              | 4 LEDs GRB           |
| WiFi LED        | Connection status | GPIO 48             | Onboard LED          |

---

## Tech Stack

### Firmware ESP32
- Framework arduino or PlatformIO  
- FreeRTOS tasks, semaphores, mutexes  
- Libraries  
  - DHT20  
  - LiquidCrystal I2C  
  - Adafruit NeoPixel  
  - ESP32Servo  

### Edge AI Raspberry Pi
- Python 3.10 or higher  
- Libraries  
  - ultralytics  
  - opencv python  
  - socket  

---

## Installation and setup

### 1. ESP32 Firmware
1. Clone repository: git clone [https://github.com/Quangminh1506/LogicDesignProject.git](https://github.com/Quangminh1506/LogicDesignProject.git)
2. Open the firmware folder in VS Code (PlatformIO) or Arduino IDE.

Modify global.cpp with your WiFi credentials:

const char* WIFI_SSID = "YOUR_WIFI_NAME";
// const char* WIFI_PASSWORD = "YOUR_WIFI_PASS"; 

3. Upload the code to ESP32-S3.

4. Monitor serial output to confirm IP address (default: 172.28.182.55).

### 2. Raspberry Pi AI setup

1. Navigate to the Python script directory.

2. Install dependencies:

pip install ultralytics opencv-python


3. Ensure the best.pt model file is in the same directory.

4. Edit detect_test.py if your ESP32 IP is different:

ESP_IP = "172.28.182.55"


### Usage

1. Power on the ESP32-S3. The LCD should initialize and display sensor readings.

2. Run the AI Client on Raspberry Pi:

python detect_test.py
