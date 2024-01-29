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
## 2. Tx code

### 2.1 code....
### 2.2 code....

---
## 3. Rx code
