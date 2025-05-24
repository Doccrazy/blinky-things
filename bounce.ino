#include "LSM6DS3.h"
#include "Wire.h"
#include "SPI.h"
#include <Adafruit_NeoPixel.h>
#include <npNeoPixel.h>
#include <npBouncingBall.h>

#define NeoPin     A0 // NeoPixel Pin
#define MAXPIXELS 52 // Number of Pixels
#define NSTRIPES 4 //Number of Stripes 

npNeoPixel pixels = npNeoPixel(MAXPIXELS, NeoPin, NEO_GRB + NEO_KHZ800, 8.0);

npVirtualNeo vNeo1(&pixels, 12, 0);
npVirtualNeo vNeo2(&pixels, 13, 25);
npVirtualNeo vNeo3(&pixels, 38, 26);
npVirtualNeo vNeo4(&pixels, 39, 51);

npBouncingBall bballs[NSTRIPES] = {npBouncingBall(50,vNeo1),npBouncingBall(60,vNeo2),
                                    npBouncingBall(40,vNeo3),npBouncingBall(20,vNeo4)};

float accel[3] = {0,0,0};
float accel_old[3] = {0,0,0};
float diff[3] = {0,0,0};
byte *c;
float Motion_Theshold = 0.02;
uint32_t time_old;

uint16_t errorsAndWarnings = 0;

//Create instance of LSM6DS3Core
LSM6DS3 myIMU(I2C_MODE, 0x6A);    //I2C device address 0x6A

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    pixels.begin();
    pixels.clear();
    pixels.npShow();

    //Init Serial port
    Serial.begin(9600);
    // while (!Serial);

    //Call .beginCore() to configure the IMU
    if (myIMU.begin() != 0) {
        Serial.print("\nDevice Error.\n");
    } else {
        Serial.print("\nDevice OK.\n");
    }

    time_old = micros();
}

void loop() {
    uint32_t time;
    float delta;
    float diffsum;
    float diff_ps;

    time = micros();
    delta = (float)(time - time_old) / 1000000.0;
    time_old = time;

    //   // turn the LED on (HIGH is the voltage level)
    // digitalWrite(LED_BUILTIN, HIGH);
    // // // wait for a second
    // delay(25);
    // // // turn the LED off by making the voltage LOW
    // digitalWrite(LED_BUILTIN, LOW);

    // delay(25);

    //Accelerometer
    accel[0] = myIMU.readFloatAccelX();
    accel[1] = myIMU.readFloatAccelY();
    accel[2] = myIMU.readFloatAccelZ();

    diff[0] = abs(accel[0] - accel_old[0]);
    diff[1] = abs(accel[1] - accel_old[1]);
    diff[2] = abs(accel[2] - accel_old[2]);
 
    accel_old[0] = accel[0]; accel_old[1] = accel[1]; accel_old[2] = accel[2];
    diffsum = diff[0] + diff[1] + diff[2];
    diff_ps = diffsum * delta;
    Serial.print(" delta = ");
    Serial.println(diff_ps, 4);

    for(int i=0;i<NSTRIPES;i++) bballs[i].update();

    if (bballs[3].hasFinished() && (diff_ps>Motion_Theshold))
    {
        c=Wheel(random(0,255));

        for(int i=0;i<NSTRIPES;i++) 
        {
            //uncomment following line for fixed collor for the balls
            //and the upper line c=Wheel... 
            bballs[i].changeColor(*c, *(c+1), *(c+2));
            bballs[i].restart();
        }
    }

    delay(5);
}

byte * Wheel(byte WheelPos) {
  static byte c[3];
 
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }

  return c;
}