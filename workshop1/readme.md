# Build your Drone controller based on ESP-NOW

---
## 1. ESP-NOW

---
## The whole code: https://github.com/UBRoboticsWorkshop/ESP-NOW-controller/tree/main
---
## 2. Get the MAC Address

> A MAC (Media Access Control) address, referred to as a hardware or physical address, is a unique, 12-character alphanumeric attribute that is used to identify individual electronic devices on a network. An example of a MAC address is: 00-B0-D0-63-C2-26.

- First, you need to know the MAC address of the Rx device you want to send to.

```cpp
#include "WiFi.h"
 
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
}
 
void loop() {
  delay(5000);
  Serial.print("My Mac is:");
  Serial.println(WiFi.macAddress());
}

```

---
## 3. Tx code
> code for the transmitter
### 3.1 header part
- include libraries
```cpp
#include <esp_now.h>
//#include <esp_wifi.h>
#include <WiFi.h>
#include <Arduino.h>
```

- creat variables: WiFi channel, Rx MAC and data for the receiver 
```cpp
// Set the wifi channel (1-13)
#define channel_ 6

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t RxMacAddr[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

struct PacketData {
  uint16_t lxAxis;
  uint16_t lyAxis;
  uint16_t rxAxis;
  uint16_t ryAxis;
 
  uint16_t ch1;
  uint16_t ch2;
  uint16_t ch3;
  uint16_t ch4;  
  uint16_t ch5;
  uint16_t ch6;
  uint16_t ch7;
  uint16_t ch8;  
} data;
```

- a function that helps to reduce the drift of the joystick.
```cpp
uint16_t ReMap(uint16_t value, bool reverse) {
  if (value >= 2200) {
    value = map(value, 2200, 4095, 2048, 4095);
  } else if (value <= 1800) {
    value = (value == 0 ? 0 : map(value, 1800, 0, 2048, 0));
  } else {
    value = 2048;
  }

  if (reverse) {
    value = 4095 - value;
  }
  //Serial.println(value);  
  return value;
}
```
### 3.2 main cpp file

---
## 3. Rx code
