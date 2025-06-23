#include "LSM6DS3.h"
#include "Wire.h"
#include "SPI.h"
#include <Adafruit_NeoPixel.h>
#include <npNeoPixel.h>
#include <npVirtualNeo.h>

#define NeoPin     A0 // NeoPixel Pin
#define MAXPIXELS 190 // Number of Pixels
#define NSTRIPES 10 //Number of Stripes 
#define NSTRIPES_HOR 19

npNeoPixel pixels = npNeoPixel(MAXPIXELS, NeoPin, NEO_GRB + NEO_KHZ800, 8.0);

npVirtualNeo* vNeoVert[NSTRIPES] = {0};
npVirtualNeo* vNeoHor[NSTRIPES_HOR] = {0};
npVirtualNeo vNeoFull(&pixels, 0, MAXPIXELS-1);

//Create instance of LSM6DS3Core
LSM6DS3 myIMU(I2C_MODE, 0x6A);    //I2C device address 0x6A
float accel[3] = {0,0,0};
float gyro[3] = {0,0,0};
float accel_sum_sq;
float gyro_sum_sq;
float gyro_avg = 0.0f;
float gyro_avg2[3] = {0,0,0};
#define GYRO_WINDOW 75

uint effectId = 0;
uint32_t t, tLock = 0;

float Motion_Switch_Theshold_Sq = pow(3.5, 2);
float Gyro_Switch_Theshold_Sq = pow(130, 2);
float Max_Gyro_Sq = pow(140, 2);

#define EFFECT_DURATION 5000

void setup() {
    for (int i = 0; i < NSTRIPES; i++) {
        vNeoVert[i] = new npVirtualNeo(&pixels, i%2 == 1 ? i*NSTRIPES_HOR : (i+1)*NSTRIPES_HOR - 1, i%2 == 1 ? (i+1)*NSTRIPES_HOR - 1 : i*NSTRIPES_HOR);
    }
    for (int i = 0; i < NSTRIPES_HOR; i++) {
        vNeoHor[i] = new npVirtualNeo(&pixels, i, MAXPIXELS - NSTRIPES + i, NSTRIPES_HOR);
    }

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

    setup_bounce();
    setup_line();
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
