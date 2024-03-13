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

  IMUHSpi->begin(17, 15, 16, 18); 
  IMUHSpi->setFrequency(1000000);
  IMUHSpi->setDataMode(SPI_MODE0);
  IMUHSpi->setBitOrder(MSBFIRST);
}