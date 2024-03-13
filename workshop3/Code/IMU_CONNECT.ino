// IMU Check Before Takeoff

#include <SPI.h>

#define MPU6500_NCS 18
#define MPU6500_ID 0x70
#define WHO_AM_I_REG 117
#define READ_BIT_MASK 0x80

SPIClass *IMUHSpi = NULL; //Create a null pointer to our SPI controller. You can imagine this as a "SPI controller box", so we can tell the ESP32S3 "use the controller you find in the box called IMUHSPI" instead of directly telling it "Use SPI controller 2"

void setup() {
  Serial.begin(); //Look ma, no baud rate! You don't actually need to set it with the S3's USB controller, but I'll often set it to 115200 as a force of habit

  delay(500); //A short delay, to let USB serial start before we try to use it
  

  IMUHSpi = new SPIClass(FSPI); //The ESP32 S3 has four SPI busses- two internal, two external. Here, we'll set the null pointer to be refereing to one of the two external ones
  setupIMUSPI(IMUHSpi); //This function sets all of the parameters of the SPI controller to work with the MPU6500

  if (readIMURegister(WHO_AM_I_REG) != MPU6500_ID) { //If the value returned from the name register isn't its name
    Serial.println("IMU not found. Takeoff may result in an unpleasant day."); //Warn the user
    Serial.print("WHO AM I register returned: 0x");
    Serial.println(readIMURegister(WHO_AM_I_REG), HEX); //This might return 0x71. If this is the case, just change the definition at the top of the sketch
    while (1) {; // Halt program on error, so the drone can't do anything funky
    }
  } else {
    Serial.println("IMU found");
  }
}

void loop() {
  // Main loop
}

int readIMURegister(int registerAddress) {
  int readCommand;
  int regData;

  //Put your code here

  return regData;
}

void setupIMUSPI(SPIClass *IMUHSpi) {
  pinMode(MPU6500_NCS, OUTPUT); 
  digitalWrite(MPU6500_NCS, HIGH);

  IMUHSpi->begin(17, 15, 16, 18);  //These are the pins connected to the MPU6500 on the Air Unit. If you hover your mouse over begin, it'll tell you what each does
  IMUHSpi->setFrequency(1000000); //Set the speed to a gentle 1 million bits per second. We'll crank it up to 20 million later
  IMUHSpi->setDataMode(SPI_MODE0);  //This is about what side of the clock you read on, the datasheet for a sensor will tell you which to use
  IMUHSpi->setBitOrder(MSBFIRST); //Send the data Most Significant Bit first. Again, datasheets will tell you how to set this
}