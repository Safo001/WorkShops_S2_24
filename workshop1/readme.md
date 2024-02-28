# Build your Drone controller based on ESP-NOW

---
As many of you found out during Sumobots, using an Access Point and TCP or UDP to communicate with your robot can cause a lot of issues- latency and disconnects being two major issues. For an FPV drone, this isn't good enough.
Today, we'll be looking at using the ESP32's built in proprietary communication systems to achive much better control. 

(You can even reuse this in sumobots next year to stomp first years)

Today's workshop will be a fairly simple reintroduction to robotics, however in our next session we will start to take a deeper look into low level programming and hardware to start using the flight controller.


## 1. ESP-NOW

<p align="center">
<img width="504" alt="Screen Shot 2024-01-30 at 1 42 46 PM" src="https://github.com/UBRoboticsWorkshop/WorkShops_S2_24/assets/110237339/8a347019-ef6e-47d4-b4ee-2bd310d4d13a">
</p>

- **What exactly is ESP-NOW? Why do we need it?**

ESP-NOW is a wireless communication protocol that allows multiple devices to connect to each other without using a central WiFi network.

- **We will also be using Arduino IDE for this project. Just to clarify, **

<p align="center">
<img width="286" alt="Screen Shot 2024-01-30 at 1 46 47 PM" src="https://github.com/UBRoboticsWorkshop/WorkShops_S2_24/assets/110237339/46bb8232-e9e3-46d9-a980-0b753890bb8c">
</p>

This is the interface you will be pasting your code on. 

- **How it works**
  You don't need an initial WiFi network. All you need is an initial pairing with another device, but after pairing, the connection is peer-to-peer.
---
## The whole code: https://github.com/UBRoboticsWorkshop/ESP-NOW-controller/tree/main
---
## 1. Build your first controller with a breadboard

<p align="center">[video](https://youtu.be/tSLb4vP00uo)</p>

> [!NOTE]
> All pins on ESP32 only accept 3.3v.

- **Schematic diagram:**
![Schematic diagram](./pictures/WhatsApp%20Image%202024-01-29%20at%2000.14.45_c7931bff.jpg)
---
> **The following part is the code used to send the message. You need to work with another group. One is the sender and one is the receiver.**

> [!NOTE]
> You need to know the MAC address of the TX and RX devices.
> You need to set the TX and RX devices on the same channel.
> You MUST NOT change the TX power.
> You can ONLY INCREASE the TX delay function.
---
## 2. Get the MAC Address

> A MAC (Media Access Control) address, referred to as a hardware or physical address, is a unique, 12-character alphanumeric attribute that is used to identify individual electronic devices on a network. An example of a MAC address is 00-B0-D0-63-C2-26.

- **First, you need to know the MAC address of the Rx device you want to send to. By flashing the following code into the receiver, you can get the MAC address.**
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
- **We also recommend you record the Tx device MAC address as well, as we need to check the identity of the sender later.**

---
## 3. Tx code
> Code for the transmitter
### 3.1 Header part (create a head file(.h) with the same name of your .ino file)
This can be done by clicking the three dots at the top right of the Arduino IDE, and then pressing new tab. Name this file ESPNOW_Transmitter.h.
- **Include libraries: We need esp_now, WiFi and Arduino library. They are all installed by default. You can simply include them.**
```cpp
#include <esp_now.h>
//#include <esp_wifi.h>
#include <WiFi.h>
#include <Arduino.h>
```

- **Create variables: WiFi channel, Rx MAC, and data for the receiver**
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

- **A function that helps to reduce the drift of the joystick.**
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
### 3.2 main cpp file (.ino)
- **Include the head file you created. (replace with your header file name.)**
```cpp
#include "ESPNOW_Transmitter.h"
```


- **Setup part:**
```cpp
void setup() {
  Serial.begin(115200);
```
- **Set WiFi into the SAT mode. Set the channel (there are 13 channels in total for 2.4 GHz WiFi). Set the power to -1 dBm**
> [!NOTE]
> Please do not change the power. The ESP32 generates a lot of heat during transmission. If you change it it will kill the chip and burn yourself.
```cpp
  WiFi.mode(WIFI_STA);
  WiFi.channel(channel_);
  WiFi.setTxPower(WIFI_POWER_MINUS_1dBm);
```
- **Turn on esp-now by add peers(The Rx device)**
```cpp
  if (esp_now_init() != ESP_OK) {   // Init ESP-NOW
    Serial.println("Error initializing ESP-NOW");
  } else {
    Serial.println("Succes: Initialized ESP-NOW");
  }

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
```
- **Enable input pins for joysticks**
```cpp
  // joystics
  pinMode(32,INPUT);
  pinMode(33,INPUT);
  pinMode(34,INPUT);
  pinMode(35,INPUT);
  //keys
  //pinMode(19,INPUT_PULLUP);
  //pinMode(21,INPUT_PULLUP);
  //pinMode(22,INPUT_PULLUP);
  //pinMode(23,INPUT_PULLUP); 
}
```
- **Loop function**
```cpp
void loop() {
```
- **Read the voltage from pins and remap the value**
```cpp

  data.lxAxis    = ReMap(analogRead(32), false);
  data.lyAxis    = ReMap(analogRead(33), false);
  data.rxAxis    = ReMap(analogRead(34), false);
  data.ryAxis    = ReMap(analogRead(35), false);
```
- **Send the message**
```cpp
  esp_now_send(RxMacAddr, (uint8_t *) &data, sizeof(data));
```
- **Delay**
```cpp
  delay(5); //chip temperature issue
}
```

---
<details>
<summary>

## 4. Rx code (Try to read and understand by yourself. Feel free to ask.)
</summary>

### 4.1 Header
```cpp
#include <esp_now.h>
//#include <esp_wifi.h>
#include <WiFi.h>
#include <Arduino.h>

// Check Tx identity
//#define CheckMac

// Set the wifi channel (1-13)
#define channel_ 6

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t TxMacAddr[] = {0xA8,0x42,0xE3,0xCD,0x5F,0x04};

// Timeout
#define SIGNAL_TIMEOUT 100 //ms
unsigned long lastRecvTime = 0;

struct SystemStatus{
  unsigned int DataUpdateSpeed;
  bool timeout;
  char StrMac[18];
} sys_status;

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

void printMAC(const uint8_t * mac_addr){
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
          mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
}
```

### 4.2 Main cpp
```cpp
#include "ESPNOW_Receiver.h"



// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
#ifdef CheckMac
  char MACAd[6];
  memcpy(&MACAd, mac, sizeof(MACAd));
  for (int i = 0; i < 6; i++) {
    if (MACAd[i] != TxMacAddr[i]) return;
  }
#endif

  snprintf(sys_status.StrMac, sizeof(sys_status.StrMac), "%02x:%02x:%02x:%02x:%02x:%02x",mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  // get RSSI
  //wifi_pkt_rx_ctrl_t wifi_rx_status;

  // check the identity of Tx

  memcpy(&data, incomingData, sizeof(data));
  lastRecvTime = millis(); 
}

void setup(){
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.channel(channel_);

  //ESP_ERROR_CHECK(esp_wifi_set_channel(channel_, WIFI_SECOND_CHAN_NONE));

  ESP_ERROR_CHECK(esp_now_init());


  esp_now_register_recv_cb(OnDataRecv);
}

void loop(){
  // check the data speed and update status
  sys_status.DataUpdateSpeed = millis() - lastRecvTime;
  if (sys_status.DataUpdateSpeed > SIGNAL_TIMEOUT ) {
    sys_status.timeout = true;
  } else {
    sys_status.timeout = false;
  }

  // print out the data
  Serial.print("ReMsg from: ");
  Serial.print(sys_status.StrMac);
  Serial.print("  Time cost:");
  Serial.print(sys_status.DataUpdateSpeed);
  Serial.print("  lx:");
  Serial.print(data.lxAxis);
  Serial.print("  ly:");
  Serial.print(data.lyAxis);
  Serial.print("  rx:");
  Serial.print(data.rxAxis);
  Serial.print("  ry:");
  Serial.print(data.ryAxis);
  // Other chennels
  Serial.println();

  // Drone control & PID loop
}
```

</details>

## 28th Feb - New Parts

![image](https://github.com/UBRoboticsWorkshop/WorkShops_S2_24/assets/110237339/b8ea64a4-15a8-471a-9684-b797bb9940c7)

NOTE: This model is a prototype and a basic idea for your team to build from. Please explore innovative ways to design your own controllers!!! It may be a good idea to think about ways that the circuitry can be encased...


Please follow this video for soldering :


(1) Solder ESP to Perfboard

https://github.com/UBRoboticsWorkshop/WorkShops_S2_24/assets/110237339/7468711a-e8a6-4a86-9586-cd83d3d7a7a9

(2) Solder Joysticks to Perfboard

https://drive.google.com/drive/folders/1UYhm-c77dL-LISOzTpersRXsNKGeuwwQ?usp=sharing



https://github.com/UBRoboticsWorkshop/WorkShops_S2_24/assets/110237339/f3f0fe57-48ea-4473-86bf-81150caa7db5




