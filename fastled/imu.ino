#include "LSM6DS3.h"

//Create instance of LSM6DS3Core
LSM6DS3 myIMU(I2C_MODE, 0x6A);    //I2C device address 0x6A

void setup_imu() {
  //Call .beginCore() to configure the IMU
  if (myIMU.begin() != 0) {
    Serial.print("\nDevice Error.\n");
  } else {
    Serial.print("\nDevice OK.\n");
  }
}
