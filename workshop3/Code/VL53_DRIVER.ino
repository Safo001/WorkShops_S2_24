#include <Wire.h>
#include "VL53.h"

const int address = 0x29;
int wireErrorVL53;

const uint8_t VL51L1X_DEFAULT_CONFIGURATION[] = {
  0x00, /* 0x2d : set bit 2 and 5 to 1 for fast plus mode (1MHz I2C), else don't touch */
  0x01, /* 0x2e : bit 0 if I2C pulled up at 1.8V, else set bit 0 to 1 (pull up at AVDD) */
  0x01, /* 0x2f : bit 0 if GPIO pulled up at 1.8V, else set bit 0 to 1 (pull up at AVDD) */
  0x01, /* 0x30 : set bit 4 to 0 for active high interrupt and 1 for active low (bits 3:0 must be 0x1), use SetInterruptPolarity() */
  0x02, /* 0x31 : bit 1 = interrupt depending on the polarity, use CheckForDataReady() */
  0x00, /* 0x32 : not user-modifiable */
  0x02, /* 0x33 : not user-modifiable */
  0x08, /* 0x34 : not user-modifiable */
  0x00, /* 0x35 : not user-modifiable */
  0x08, /* 0x36 : not user-modifiable */
  0x10, /* 0x37 : not user-modifiable */
  0x01, /* 0x38 : not user-modifiable */
  0x01, /* 0x39 : not user-modifiable */
  0x00, /* 0x3a : not user-modifiable */
  0x00, /* 0x3b : not user-modifiable */
  0x00, /* 0x3c : not user-modifiable */
  0x00, /* 0x3d : not user-modifiable */
  0xff, /* 0x3e : not user-modifiable */
  0x00, /* 0x3f : not user-modifiable */
  0x0F, /* 0x40 : not user-modifiable */
  0x00, /* 0x41 : not user-modifiable */
  0x00, /* 0x42 : not user-modifiable */
  0x00, /* 0x43 : not user-modifiable */
  0x00, /* 0x44 : not user-modifiable */
  0x00, /* 0x45 : not user-modifiable */
  0x20, /* 0x46 : interrupt configuration 0->level low detection, 1-> level high, 2-> Out of window, 3->In window, 0x20-> New sample ready , TBC */
  0x0b, /* 0x47 : not user-modifiable */
  0x00, /* 0x48 : not user-modifiable */
  0x00, /* 0x49 : not user-modifiable */
  0x02, /* 0x4a : not user-modifiable */
  0x0a, /* 0x4b : not user-modifiable */
  0x21, /* 0x4c : not user-modifiable */
  0x00, /* 0x4d : not user-modifiable */
  0x00, /* 0x4e : not user-modifiable */
  0x05, /* 0x4f : not user-modifiable */
  0x00, /* 0x50 : not user-modifiable */
  0x00, /* 0x51 : not user-modifiable */
  0x00, /* 0x52 : not user-modifiable */
  0x00, /* 0x53 : not user-modifiable */
  0xc8, /* 0x54 : not user-modifiable */
  0x00, /* 0x55 : not user-modifiable */
  0x00, /* 0x56 : not user-modifiable */
  0x38, /* 0x57 : not user-modifiable */
  0xff, /* 0x58 : not user-modifiable */
  0x01, /* 0x59 : not user-modifiable */
  0x00, /* 0x5a : not user-modifiable */
  0x08, /* 0x5b : not user-modifiable */
  0x00, /* 0x5c : not user-modifiable */
  0x00, /* 0x5d : not user-modifiable */
  0x01, /* 0x5e : not user-modifiable */
  0xcc, /* 0x5f : not user-modifiable */
  0x0f, /* 0x60 : not user-modifiable */
  0x01, /* 0x61 : not user-modifiable */
  0xf1, /* 0x62 : not user-modifiable */
  0x0d, /* 0x63 : not user-modifiable */
  0x01, /* 0x64 : Sigma threshold MSB (mm in 14.2 format for MSB+LSB), use SetSigmaThreshold(), default value 90 mm  */
  0x68, /* 0x65 : Sigma threshold LSB */
  0x00, /* 0x66 : Min count Rate MSB (MCPS in 9.7 format for MSB+LSB), use SetSignalThreshold() */
  0x80, /* 0x67 : Min count Rate LSB */
  0x08, /* 0x68 : not user-modifiable */
  0xb8, /* 0x69 : not user-modifiable */
  0x00, /* 0x6a : not user-modifiable */
  0x00, /* 0x6b : not user-modifiable */
  0x00, /* 0x6c : Intermeasurement period MSB, 32 bits register, use SetIntermeasurementInMs() */
  0x00, /* 0x6d : Intermeasurement period */
  0x0f, /* 0x6e : Intermeasurement period */
  0x89, /* 0x6f : Intermeasurement period LSB */
  0x00, /* 0x70 : not user-modifiable */
  0x00, /* 0x71 : not user-modifiable */
  0x00, /* 0x72 : distance threshold high MSB (in mm, MSB+LSB), use SetD:tanceThreshold() */
  0x00, /* 0x73 : distance threshold high LSB */
  0x00, /* 0x74 : distance threshold low MSB ( in mm, MSB+LSB), use SetD:tanceThreshold() */
  0x00, /* 0x75 : distance threshold low LSB */
  0x00, /* 0x76 : not user-modifiable */
  0x01, /* 0x77 : not user-modifiable */
  0x0f, /* 0x78 : not user-modifiable */
  0x0d, /* 0x79 : not user-modifiable */
  0x0e, /* 0x7a : not user-modifiable */
  0x0e, /* 0x7b : not user-modifiable */
  0x00, /* 0x7c : not user-modifiable */
  0x00, /* 0x7d : not user-modifiable */
  0x02, /* 0x7e : not user-modifiable */
  0xc7, /* 0x7f : ROI center, use SetROI() */
  0xff, /* 0x80 : XY ROI (X=Width, Y=Height), use SetROI() */
  0x9B, /* 0x81 : not user-modifiable */
  0x00, /* 0x82 : not user-modifiable */
  0x00, /* 0x83 : not user-modifiable */
  0x00, /* 0x84 : not user-modifiable */
  0x01, /* 0x85 : not user-modifiable */
  0x00, /* 0x86 : clear interrupt, use ClearInterrupt() */
  0x00  /* 0x87 : start ranging, use StartRanging() or StopRanging(), If you want an automatic start after VL53L1X_init() call, put 0x40 in location 0x87 */
};

void writeReg(uint16_t reg, uint8_t value){
  Wire.beginTransmission(address);
  Wire.write((uint8_t)(reg >> 8)); // reg high byte
  Wire.write((uint8_t)(reg));      // reg low byte
  Wire.write(value);
  wireErrorVL53 = Wire.endTransmission();
}


void writeReg16(uint16_t reg, uint16_t value){
  Wire.beginTransmission(address);
  Wire.write((uint8_t)(reg >> 8)); // reg high byte
  Wire.write((uint8_t)(reg));      // reg low byte
  Wire.write((uint8_t)(value >> 8)); // value high byte
  Wire.write((uint8_t)(value)); // value low byte
  wireErrorVL53 = Wire.endTransmission();
}

uint8_t readReg(uint16_t reg){
  uint8_t val;

  Wire.beginTransmission(address);
  Wire.write((uint8_t)(reg >> 8)); // reg high byte
  Wire.write((uint8_t)(reg));      // reg low byte
  wireErrorVL53 = Wire.endTransmission();

  Wire.requestFrom(address, (uint8_t)1);
  val = Wire.read();

  return val;
}

uint16_t readReg16(uint16_t reg){
  uint16_t val;

  Wire.beginTransmission(address);
  Wire.write((uint8_t)(reg >> 8)); // reg high byte
  Wire.write((uint8_t)(reg));      // reg low byte
  wireErrorVL53 = Wire.endTransmission();

  Wire.requestFrom(address, (uint8_t)2);
  val  = (uint16_t)Wire.read() << 8; // value high byte
  val |=           Wire.read();      // value low byte

  return val;
}

int getRange(){
	uint16_t distance;

  distance = readReg16(VL53L1_RESULT__FINAL_CROSSTALK_CORRECTED_RANGE_MM_SD0);
	return distance;
}

void startRanging(){
	writeReg(SYSTEM__MODE_START, 0x40);	//LiDAR time :D
}

void stopRanging(){
	writeReg(SYSTEM__MODE_START, 0x00);	//LiDAR become no :(
}

void clearInterrupt(){ //Turn off the interrupt GPIO pin, which may or may not be used. Either way, I connected it on the FC
  writeReg(SYSTEM__INTERRUPT_CLEAR, 0x01);
}

int getIntPolarity(){
  int value;
  value = readReg(GPIO_HV_MUX__CTRL);
  value &= 0x10;
  return !(value>>4); 
}

boolean dataReady(){
  int intPolarity;
  int intStatus;
  intPolarity = getIntPolarity();
  intStatus = readReg(GPIO__TIO_HV_STATUS);
  
		if ((intStatus & 1) == intPolarity)
			return 1;
		else
			return 0;
	

}

uint8_t getRangeStatus(){
	uint8_t RgSt, rangeStatus;

	rangeStatus = 255;
  RgSt = readReg(VL53L1_RESULT__RANGE_STATUS);
	RgSt = RgSt & 0x1F;
	if (RgSt < 24)
		rangeStatus = status_rtn[RgSt];
	return rangeStatus;
}

void initLiDAR(){
	uint8_t reg = 0x00, tmp;

	for (reg = 0x2D; reg <= 0x87; reg++){
		writeReg(reg, VL51L1X_DEFAULT_CONFIGURATION[reg - 0x2D]);
	}
	startRanging();
	tmp  = 0;
	while(!dataReady()){;}
	clearInterrupt();
	stopRanging();
	writeReg(VL53L1_VHV_CONFIG__TIMEOUT_MACROP_LOOP_BOUND, 0x09); /* two bounds VHV */
	writeReg(0x0B, 0); /* start VHV from the previous temperature */

  delay(100);

  startRanging();

}

int bootState(){
	uint8_t booted;

	booted = readReg(VL53L1_FIRMWARE__SYSTEM_STATUS);

  Serial.println(booted);
	
	return booted & 0x01;
}

void setup() {
  Serial.begin(115200);
  pinMode(12, INPUT); //This pin can be used to check for new data, which is faster than using the function

  Wire.begin(13,14);
  Wire.setClock(400000);

  bool booted = false;
  while (!booted) {
    //Put something here to set booted to true when the VL53 is booted
    delay(10);
  }

  //Flash the LiDAR's firmware here

  delay(100);
}

uint16_t range, status;
int startTime, endTime;
void loop(){
  while (!false){ //Make this check for new data, and exit the loop when there is new data
    delay(50);
  }

  startTime = micros();
  range = 0; //Set this to the actual range
  status = 0; //Set this to the status
  
  endTime = micros();

  Serial.print(range);
  Serial.print(" ");
  Serial.print(status);
  Serial.print(" ");
  Serial.println(endTime-startTime);

}
