#include "LSM6DS3.h"
#include "Wire.h"
#include "SPI.h"
#include <Adafruit_NeoPixel.h>
#include <npNeoPixel.h>
#include <npVirtualNeo.h>

#define NeoPin     A0 // NeoPixel Pin
#define MAXPIXELS 285 // Number of Pixels
#define NSTRIPES 19 //Number of Stripes 
#define NSTRIPES_HOR 15

npNeoPixel pixels = npNeoPixel(MAXPIXELS, NeoPin, NEO_GRB + NEO_KHZ800, 8.0);

npVirtualNeo vNeo1(&pixels, 0, 14);
npVirtualNeo vNeo2(&pixels, 29, 15);
npVirtualNeo vNeo3(&pixels, 30, 44);
npVirtualNeo vNeo4(&pixels, 59, 45);
npVirtualNeo vNeo5(&pixels, 60, 74);
npVirtualNeo vNeo6(&pixels, 89, 75);
npVirtualNeo vNeo7(&pixels, 90, 104);
npVirtualNeo vNeo8(&pixels, 119, 105);
npVirtualNeo vNeo9(&pixels, 120, 134);
npVirtualNeo vNeo10(&pixels, 149, 135);
npVirtualNeo vNeo11(&pixels, 150, 164);
npVirtualNeo vNeo12(&pixels, 179, 165);
npVirtualNeo vNeo13(&pixels, 180, 194);
npVirtualNeo vNeo14(&pixels, 209, 195);
npVirtualNeo vNeo15(&pixels, 210, 224);
npVirtualNeo vNeo16(&pixels, 239, 225);
npVirtualNeo vNeo17(&pixels, 240, 254);
npVirtualNeo vNeo18(&pixels, 269, 255);
npVirtualNeo vNeo19(&pixels, 270, 284);

npVirtualNeo vNeoHor1(&pixels, 0, 270, 15);
npVirtualNeo vNeoHor2(&pixels, 1, 271, 15);
npVirtualNeo vNeoHor3(&pixels, 2, 272, 15);
npVirtualNeo vNeoHor4(&pixels, 3, 273, 15);
npVirtualNeo vNeoHor5(&pixels, 4, 274, 15);
npVirtualNeo vNeoHor6(&pixels, 5, 275, 15);
npVirtualNeo vNeoHor7(&pixels, 6, 276, 15);
npVirtualNeo vNeoHor8(&pixels, 7, 277, 15);
npVirtualNeo vNeoHor9(&pixels, 8, 278, 15);
npVirtualNeo vNeoHor10(&pixels, 9, 279, 15);
npVirtualNeo vNeoHor11(&pixels, 10, 280, 15);
npVirtualNeo vNeoHor12(&pixels, 11, 281, 15);
npVirtualNeo vNeoHor13(&pixels, 12, 282, 15);
npVirtualNeo vNeoHor14(&pixels, 13, 283, 15);
npVirtualNeo vNeoHor15(&pixels, 14, 284, 15);

npVirtualNeo vNeoFull(&pixels, 0, 284);

//Create instance of LSM6DS3Core
LSM6DS3 myIMU(I2C_MODE, 0x6A);    //I2C device address 0x6A
float accel[3] = {0,0,0};
float gyro[3] = {0,0,0};
float accel_sum_sq;
float gyro_sum_sq;
float gyro_avg = 0.0f;
float gyro_avg2[3] = {0,0,0};
#define GYRO_WINDOW 50

uint effectId = 0;
uint32_t t, tLock = 0;

float Motion_Switch_Theshold_Sq = pow(3.25, 2);
float Gyro_Switch_Theshold_Sq = pow(130, 2);
float Max_Gyro_Sq = pow(140, 2);

#define EFFECT_DURATION 5000

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    pixels.begin();
    pixels.clear();
    pixels.npShow();

    //Init Serial port
    Serial.begin(9600);
    //while (!Serial);

    //Call .beginCore() to configure the IMU
    if (myIMU.begin() != 0) {
        Serial.print("\nDevice Error.\n");
    } else {
        Serial.print("\nDevice OK.\n");
    }

    //setup_bluetooth();
}

void loop() {

    //Accelerometer
    accel[0] = myIMU.readFloatAccelX();
    accel[1] = myIMU.readFloatAccelY();
    accel[2] = myIMU.readFloatAccelZ();

    //Gyro
    gyro[0] = myIMU.readFloatGyroX();
    gyro[1] = myIMU.readFloatGyroY();
    gyro[2] = myIMU.readFloatGyroZ();

    gyro_avg2[0] = (gyro_avg2[0] * (GYRO_WINDOW-1) + gyro[0]) / GYRO_WINDOW;
    gyro_avg2[1] = (gyro_avg2[1] * (GYRO_WINDOW-1) + gyro[1]) / GYRO_WINDOW;
    gyro_avg2[2] = (gyro_avg2[2] * (GYRO_WINDOW-1) + gyro[2]) / GYRO_WINDOW;

    accel_sum_sq = accel[0]*accel[0] + accel[1]*accel[1] + accel[2]*accel[2];
    gyro_avg = min(gyro_avg2[0]*gyro_avg2[0] + gyro_avg2[1]*gyro_avg2[1] + gyro_avg2[2]*gyro_avg2[2], Max_Gyro_Sq);

    t = millis();

    //Serial.print(accel_sum_sq);
    //Serial.print(", ");
    //Serial.println(gyro_avg);
    //delay(50);

    if (gyro_avg > Gyro_Switch_Theshold_Sq) {
        switchEffect(2);

        // Serial.print(accel_sum_sq);
        // Serial.print(", ");
        // Serial.print(gyro_avg);
        // Serial.println("Switch gyro");
    } else if (accel_sum_sq > Motion_Switch_Theshold_Sq) {
        switchEffect(1);

        // Serial.print(accel_sum_sq);
        // Serial.print(", ");
        // Serial.print(gyro_avg);
        // Serial.println("Switch accel");
    } else {
        switchEffect(0);
    }

    if (effectId == 0) {
        run_rainbow();
    } else if (effectId == 1) {
        run_bounce();
    } else if (effectId == 2) {
        run_line();
    }
}

void switchEffect(uint effect) {
    if ((tLock != 0 && t < tLock) || effectId == effect) {
        return;
    }
    pixels.clear();
    effectId = effect;
    if (effect != 0) {
        pixels.setBrightness(255);
        tLock = t + EFFECT_DURATION;
    } else {
        pixels.setBrightness(64);
    }

    if (effectId == 0) {
        restart_rainbow();
    } else if (effectId == 1) {
        restart_bounce();
    } else if (effectId == 2) {
        restart_line();
    }
}

void bleuart_receive(char* str) {
    effectId = (effectId+1)%2;

    pixels.clear();
    pixels.npShow();
}
