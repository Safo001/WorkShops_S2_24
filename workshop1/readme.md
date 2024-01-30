# Build your Drone controller based on ESP-NOW

---
## 1. ESP-NOW
<img width="504" alt="Screen Shot 2024-01-30 at 1 42 46 PM" src="https://github.com/UBRoboticsWorkshop/WorkShops_S2_24/assets/110237339/8a347019-ef6e-47d4-b4ee-2bd310d4d13a">
What exactly is ESP-NOW? Why do we need it?
ESP-NOW is a wireless communication protocol that allows multiple devices to connect to each other without using a Router's WiFi network.

We will also be using Arduino IDE for this project. Just to clarify, 

<img width="286" alt="Screen Shot 2024-01-30 at 1 46 47 PM" src="https://github.com/UBRoboticsWorkshop/WorkShops_S2_24/assets/110237339/46bb8232-e9e3-46d9-a980-0b753890bb8c">
This is the interface you will be pasting your code on. 
---
## The whole code: https://github.com/UBRoboticsWorkshop/ESP-NOW-controller/tree/main
---
## 1. Build your first controller with bread board
![video](./)

- All pins on ESP32 only accept 3.3v.
![Schematic diagram](./pictures/WhatsApp%20Image%202024-01-29%20at%2000.14.45_c7931bff.jpg)
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
- include the head file
```cpp
#include "ESPNOW_Transmitter.h"
```


- turn on the WiFi
    - set it into the SAT mode
    - set the channel: there are 13 channels in total for 2.4Ghz WiFi
- turn on the sep now
    - Register peer
    - add the peer (the Rx device)
- enable input pins
```cpp
void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.setTxPower(WIFI_POWER_MINUS_1dBm);
  WiFi.channel(channel_);

  //esp_wifi_start();
  //ESP_ERROR_CHECK(esp_wifi_set_channel(channel_, WIFI_SECOND_CHAN_NONE));

  if (esp_now_init() != ESP_OK) {   // Init ESP-NOW
    Serial.println("Error initializing ESP-NOW");
  } else {
    Serial.println("Succes: Initialized ESP-NOW");
  }

  //esp_wifi_config_espnow_rate();
  
  // Register peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, RxMacAddr, 6);
  peerInfo.channel = 0;  //current WiFi chennel
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  } else {
    Serial.println("Succes: Added peer");
  } 

  //esp_now_get_peer();

  // enable input pins
  pinMode(15,INPUT_PULLUP);
  pinMode(16,INPUT_PULLUP);
  pinMode(17,INPUT_PULLUP);
  pinMode(18,INPUT_PULLUP); 
  pinMode(19,INPUT_PULLUP);
  pinMode(21,INPUT_PULLUP);
  pinMode(22,INPUT_PULLUP);
  pinMode(23,INPUT_PULLUP); 
}
```

- read the voltage form pins and remap the value
- send the message
- delay
```cpp
void loop() {
  data.lxAxis    = ReMap(analogRead(32), false);
  data.lyAxis    = ReMap(analogRead(33), false);
  data.rxAxis    = ReMap(analogRead(34), false);
  data.ryAxis    = ReMap(analogRead(35), false);

  // Other channels
  // data.ch1  = !digitalRead(15);
  // data.ch2   = !digitalRead(16);
  // data.ch3   = !digitalRead(17);
  // data.ch4   = !digitalRead(18);
  // data.ch5   = !digitalRead(19);
  // data.ch6   = !digitalRead(21);
  // data.ch7   = !digitalRead(22);
  // data.ch8   = !digitalRead(23);  
  
  esp_now_send(RxMacAddr, (uint8_t *) &data, sizeof(data));
  delay(5); //chip temperature issue
}

```

---
## 3. Rx code
