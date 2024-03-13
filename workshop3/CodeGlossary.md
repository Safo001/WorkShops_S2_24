Code Dictionary

```cpp
void neopixelWrite(uint8_t pin, uint8_t red_val, uint8_t green_val, uint8_t blue_val)
```
Sends a data packet to an addressable LED. This accepts a pin number, and 3 values from 0-255 to set the brightness of each colour.
This uses RMT, which uses long pulses and short pulses to represent 1 and 0 respectively.

```cpp
unsigned long micros()
```
Returns the number of microseconds the sketch has been running


```cpp
uint8_t transfer(uint8_t data);
```

Transmits a byte of data on MOSI, and listens for a byte of data on MISO

```cpp
void digitalWrite(uint8_t pin, uint8_t val)
```

Sets pin number “pin” to “val”. False, LOW or 0 sets it low, while anything else sets it high.

```cpp
int variableToShift << int placesToShift 
```

Left shift- moves each bit in variableToShift left, placesToShift times. This is equivalent to multiplying "variableToShift" by 2^ "placesToShift" with integers.


```cpp
int variableA | int variableB
```
Bit-wise or- combines two variables by performing an "or" function on each pair of bits. 
