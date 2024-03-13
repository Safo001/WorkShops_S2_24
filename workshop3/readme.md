# Using the Air Unit

The Air Unit is similar to the ESP32 development board from Sumobots, however with additional systems on board to support flight. Some of the features of the Air Unit include:

- LiDAR, for millimetre altitude control
-	IMU, for rotation sensing to stabilize flight
-	Switching regulator, to allow direct supply from lithium batteries
-	8 GPIO pins broken out- these have a very small spacing though, so only try to use these if you’re very confident in soldering
-	USB-C, because at UBRobotics we live large 
-	RGB LED, added because our President Aswathy is a Gamer and she threatened to kick me off the committee if I didn’t make the drones replicate the exact colour fade she uses on her Cherry MX 2.0S Red Gaming Keyboard (with mechanical switches). Making the LED cover the full Adobe Wide Gamut RGB was the hardest part of the design, so you’d better appreciate it.

Through this tutorial, make sure to use the code glossary included. 

# Step 1: Connect to the ESP32-S3
The Air Unit uses an ESP32-S3, which is a major upgrade from the base ESP32. One of the benefits is direct connection to USB, without a programming chip.

However, you’ll need to bootstrap it the first time. You can do this simply by holding down the BOOT button as you plug it in, to put it into boot mode. In this mode, it won’t run any code- it’ll just wait for a download.

Now you should see a port available for download and can proceed as normal. Under “Tools”, set the following settings:

<img width="600" src="https://github.com/UBRoboticsWorkshop/WorkShops_S2_24/blob/main/Images/UploadError.png">
 
Hit upload with a blank sketch to save these settings to the ESP32-S3. If you can’t get the board to print text, make sure these settings are correct.

# Step 2: Hello World
From here, you can make an new sketch and write a simple “Hello World” program to check that the ESP32-S3 is working:

```cpp

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println("Hello World");
}
```

If successful, the unit will print Hello World very quickly into the serial monitor- the system is working. 

However, if you attempt to upload a new sketch you will probably see something like this:
 
This is because the USB connection is overloaded. This is a possible issue with the ESP32-S3 onboard USB, but it can be fixed by using the BOOT button.

Add a 1000ms delay into the sketch after the print statement, and then unplug the ESP32-S3. Hold down the button marked BOOT, plug in the USB cable and then release the BOOT button. You should be able to see this message in the serial terminal:
 
Uploading should now be successful, and the program will run after you reboot the ESP32-S3- this time printing the text once per second. From here, you can upload the code without using the BOOT button. If there are issues with uploading to the ESP32-S3, try the BOOT button to recover it.

When working with UAVs, the main loop will run thousands of times per second, which means a print statement in the main loop can cause the ESP32-S3 to lock up.

You can also tap the reset button instead of unplugging the Air Unit.

# Step 3: Using the LED

This section is on the LED_TEST sketch.

Unlike the LEDs last semester, the Air Unit has a WS2812 addressable LED allowing digital colour and brightness control. It’s also a little bit brighter than a regular LED.

Use neopixelWrite() to set the colour of the LED, which is connected to pin 48. 

You can use (brightness+brightness*sin(i)/2) to have the brightness of each channel fade smoothly between zero and brightness.

```cpp
const int brightness = 27;

void setup() {
 
}

float i;
void loop() {
  delay(10);
  i+=0.1;
  //Put your neopixel code here
}
```

To test how long this command takes, create a variable called:

long startTime, endTime;

Set startTime to micros() before the code to be measured, and endTime to micros() once it has run. You can then read out the difference between the two out on the serial monitor. How long does this command take?

For dynamic systems, it’s important to make sure that your code can run fast enough to react to changes in the environment. The ESP32 has two cores, so you can put code like this which isn’t time critical on one, and the more important code on the other.

# The IMU

Simple sensors, such as a thermistor, use analog signals to communicate; in more advanced systems it’s common to find serial communications used. The Inertial Measurement Unit (IMU) on this board uses Serial Peripheral Interface (SPI) to communicate.

## Registers

Most serial devices use what is known as a register map. The register map for the MPU6500 can be found here:

https://invensense.tdk.com/download-pdf/mpu-6500-register-map/

A register map is like a table, where each “register” is a location which holds a value- these are effectively variables stored on the sensor. These values can either control the behaviour of the sensor, or hold information to be read. Some registers can be both written and read, while others have controlled access.

## SPI
The Serial Peripheral Interface uses four lines:
 
Credit: Analog Devices

•	The CS line allows for a device to be selected without an address being broadcast, which saves time
•	MOSI is data travelling out from the ESP32-S3
•	MISO is data travelling out from the IMU
•	SCLK is the clock signal, which allows for data to be read on the MOSI/MISO lines at the right time

The IMU can run SPI at 2,000,000 bits per second for all of its registers, and 20,000,000 for the main data registers.

If you want to learn more about SPI, have a look at this:

https://learn.sparkfun.com/tutorials/serial-peripheral-interface-spi/programming-for-spi 

# Step 4: Reading From the IMU	

This section is in the IMU_CONNECT sketch.

The following is a snippet of code that is used to check the IMU is working before the aircraft takes off:

```cpp
// IMU Check Before Takeoff

#include <SPI.h>

#define MPU6500_NCS 18
#define MPU6500_ID 0x70
#define WHO_AM_I_REG 117
#define READ_BIT_MASK 0x80

SPIClass *IMUHSpi = NULL;

void setup() {
  Serial.begin(115200);

  IMUHSpi = new SPIClass(FSPI);
  setupIMUSPI(IMUHSpi);

  if (readIMURegister(WHO_AM_I_REG) != MPU6500_ID) {
    Serial.println("IMU not found. Takeoff may result in an unpleasant day.");
    Serial.print("WHO AM I register returned: 0x");
    Serial.println(readIMURegister(WHO_AM_I_REG), HEX);
    while (1) {
      ; // Halt program on error
    }
  } else {
    Serial.println("IMU found");
  }
}

void loop() {
  // Main loop
}

int readIMURegister(int registerAddress) {
  int readCommand = 0;
  int regData = 0;
  return regData;
}

void setupIMUSPI(SPIClass *IMUHSpi) {
  pinMode(MPU6500_NCS, OUTPUT);
  digitalWrite(MPU6500_NCS, HIGH);

  IMUHSpi->begin(17, 15, 16, 18);  // Customize these pin numbers as needed
  IMUHSpi->setFrequency(1000000);
  IMUHSpi->setDataMode(SPI_MODE0);
  IMUHSpi->setBitOrder(MSBFIRST);
}
```

In this code, the function readIMURegister() takes a register to read and returns the value of the register. 

Run this code, and check what is said in the serial monitor. The IMU won’t be found, because readIMURegister() doesn’t do anything (causing “ an unpleasant day”).

Using digtalWrite() and IMUHSpi->transfer():

1.	Set the NCS pin to low
2.	Send the register address with the first bit set to one (to indicate a write command) to the IMU
3.	Transfer 0x00 to the IMU, and save the response to regData to be returned 
4.	Set the NCS pin back to high

When it works, it should print “IMU found”. Some of the IMUs may return a value of 0x71, if this is the case just change the definition.

Hint
To make this work, use digtalWrite() to set MPU6500_NCS (Pin 18) to LOW, which tells the IMU to expect communications. 

An outgoing message can be either a read command or a write command, identified by the first bit being a 1 for read or 0 for write.

registerAddress will be a value from 0-127. It’ll need to be a value from 128-255 to read. We can use Boolean operations to make this so- this is known as “setting a bit”. To set the leftmost bit to one, we can use: 

registerAddress | 0b10000000

0b tags the number as a binary number. However, this is not very readable so we can use hexadecimal instead.

registerAddress | 0x80

This is the same number (127), but represented as hexadecimal. Using compiler commands we can make it even more readable. There is a line at the top of the sketch:

#define READ_BIT_MASK 0x80

This line of code will not be uploaded to the ESP32 S3. Instead, it tells the compiler to replace every instance of “READ_BIT_MASK” with “0x80”. Be careful- it won’t take context into account! If you misuse compiler commands, for example defining a value for “int”, the compiler will replace every single “int” with this definition of it.

Use  IMUHspi->transfer() to send reg | 0x80. Then, send 0x00 and save the number it returns to regData. Note that because we’re using a pointer to refer to the SPI bus instead of using it directly, we use “->” rather than “.” to address member functions. 

Finally, set pin MPU6500_NCS to HIGH, in order to end the communication.

# Step 5: Read Faster

Use the IMU_SEQUENTIAL_READ sketch for this part.

If we want to read the IMU data, there are 12 registers to read- 6 different values, with two bytes each. It’s not very quick to read each register, end the communication, start it again, address the next register, so on, so on.

Instead, we can simply read the first register and then keep sending 0x00s. This will trigger a sequential read, moving down the register table each time. Not all devices support this, but all of the good ones do.

Annoyingly, Invensense decided to put thermometer data right in the middle of the data we actually care about. Great prank guys.

However, it’s still quicker to read all 14 bytes at once than two blocks of 6, so I guess your drone now doubles as an Internet of Things home assistant AI central heating blockchain enabled flying thermostat or something. 

Anyway, here’s the code:

```cpp
#include <SPI.h>

#define MPU6500_NCS 18
#define MPU6500_ACC_X_L 59
#define READ_BIT_MASK 0x80

SPIClass *IMUHSpi = NULL;
byte imuDataRawAccGyro[14];

void setup() {
  Serial.begin(115200);

  IMUHSpi = new SPIClass(FSPI);
  setupIMUSPI(IMUHSpi);
}

void loop() {
  readIMUSPI(59, 14, imuDataRawAccGyro);
  for (int i = 0; i < 14; i++) {
    Serial.print(imuDataRawAccGyro[i]);
    Serial.print("\t");
  }
  Serial.println();

  delay(250);
}

void readIMUSPI(int registerAddress, int count, byte data[]) {
	
}

void setupIMUSPI(SPIClass *IMUHSpi) {
  pinMode(MPU6500_NCS, OUTPUT);
  digitalWrite(MPU6500_NCS, HIGH);

  IMUHSpi->begin(17, 15, 16, 18);  // Customize these pin numbers as needed
  IMUHSpi->setFrequency(1000000);
  IMUHSpi->setDataMode(SPI_MODE0);
  IMUHSpi->setBitOrder(MSBFIRST);
}

```

Once again, fill in the blanks for readIMUSPI(int reg, int count, byte data[])
1.	Pull CS low
2.	Send a register address, with first bit set to indicate “read”
3.	Iterate through the data[] array with a for loop, transferring 0x00 and storing the responses in data[i]
4.	Pull CS low high to end the communication

Since all arrays are passed by reference, any changes made to data[] will persist outside readIMUSPI()- there’s no need to return anything.

When this code works, you should see a ton of numbers being printed in the Serial terminal- they won’t seem to mean anything, but will change when the board is moved.

Except the two bytes for temperature. They’ll change when the temperature changes. Thanks Invensense.

# Step 6: Processing the Data

Keep using the IMU_SEQUENTIAL_READ sketch for this part.

The data is returned as a high byte and a low byte- these are two 8 bit numbers that can be concatenated (added end to end) in binary to form one signed 16 bit number, as shown below:

|           | -32768 | 16384 | 8192 | 4096 | 2048 | 1024 | 512 | 256 | 128 | 64 | 32 | 16 | 8 | 4 | 2 | 1 |
|-----------|--------|-------|------|------|------|------|-----|-----|-----|----|----|----|--|---|---|---|
| High Byte | 1      | 0     | 0    | 1    | 1    | 0    | 1   | 0   |     |    |    |    |  |   |   |   |
| Low Byte  |        |       |      |      |      |      |     |     | 1   | 1  | 1  | 0  | 1 | 0 | 1 | 1 |


the high byte is 10011010 in binary which represents 154 in decimal.

the low byte is 11101011 in binary which represents 235 in decimal.

When we add them together, the first bit becomes negative creating -25877 in decimal.

The reason the numbers are so erratic is that some are high bytes, others low bytes, and all unsigned while the data is signed.

To process this, we’ll use the left bit shift function << and or function |. You can use the register map to see what data is stored where.

Create seven int16_t variables, named with their contents. Then for each left shift the high byte and combine it with the low byte. Finally, print them all out so they can be read.

<details>
  <summary> <b> Hint: Combining Bytes </b> </summary>
  
  The high byte must be moved 8 places left, so use: 
  
  ```cpp
  int16_t data1;
  data1 = imuDataRawAccGyro[0] << 8 | imuDataRawAccGyro[1];
```
to combine the two readings into one 16 bit number that can be positive or negative.

</details>

<details>
  <summary> <b> Hint: Register Maps  </b>  </summary>
  
The read starts at 59 and has a length of 14. This means the following registers are read:
 
The variables, in order, should be:

```cpp
int16_t accX, accY, accZ, temperature, gyroX, gyroY, gyroZ;
```

_H means high byte, and _L means low byte. The array will have them in order, so simply assign 0 and 1 to accX, 2 and 3 to accY and so on.

</details>





## Interpreting the Results

The accelerometer data measures linear acceleration, which lets it pick up both gravity and motion. The gyroscope picks up angular velocity.

If you rotate the board 90 degrees horizontally, you should see one ACC value becomes zero whilst another becomes high. While the board is rotating, one of the gyro channels will be high but will fall to zero when the rotation stops.

In the sensor fusion workshop, we’ll look at how to eliminate errors from this data and process it into something we can react to.

# Using the LiDAR

The board has a VL53L1 LiDAR, with up to 8 meters of range in ideal conditions. This will allow for your drone to maintain a steady height, making learning to fly much easier.

The LiDAR uses I2C, which is slower than SPI but uses less pins and is more suitable for large numbers of sensors which are only read occasionally. Our gyro will be read at 2KHz, while the LiDAR will only be read at about 30Hz.

For this one, I have ported the STMicroelectronics driver to be compatible with ESP32. Instead of programming a driver, like with the MPU6500, you’ll be working with the API.

This is very good news for you because there is no register map for the VL53L1. The reason for this, and I promise robotics member that I would not lie to you, is: 
 

# Step 7: Using the VL53L1 ULD

For this step, use the VL53_DRIVER sketch. This also has a header file where a lot of register names are defined, so I've packed them together as a .ZIP file. You'll need to uncompress this to open the sketch.

A useful feature on the Arduino IDE 2.0 is code folding- hover to the left of a function and an arrow should appear. Click this to minimize the	 function to one line, making it easier to quickly read over the code. You can use this to collapse all of the API functions, so that you can quickly read them in the next section and find which function corresponds to each step.

In setup, add a line of code to the while loop to set the booted variable to the boot state of the VL53. Then, call the iniLiDAR function to flash the default settings to the VL53.

In the main loop, make the while loop check if data is ready. next use the corresponding functions to set range and status. Range will be the distance in mm, and status will return 0 if the data is good.

Once you’ve done that, clear the interrupt to have the VL53 start another read.

One last thing- there should be a little plastic cover over your LiDAR. The optics are somewhat fragile, and this is there to protect them. However, to get accurate readings you’ll need to permanently remove this by peeling it off with the small tab below the optics. It might have already fallen off, so be careful not to scratch the lens.

# Next steps
The main thing to focus on from here is working on your drone frame. Remember, it's not just about being strong enough to fly- you'll also want it to survive a crash and be rigid enough to suppress vibrations.

Next session, we'll be looking at how to turn these numbers into useful flying. However, you'll want to make your drone frame now so that you have time to tweak and improve it.
