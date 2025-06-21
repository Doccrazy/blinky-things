#include "imu.h"

#include <LSM6DS3.h>
#include <SPI.h>

// Create instance of LSM6DS3Core
LSM6DS3 myIMU(I2C_MODE, 0x6A); // I2C device address 0x6A

void setup_imu() {
  // Initialize the IMU sensor
  if (myIMU.begin() != 0) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  Serial.println("IMU initialized successfully.");
}
