#include <npNeoPixel.h>
#include <npBouncingBall.h>

npBouncingBall bballs[NSTRIPES] = {npBouncingBall(50,vNeo1),npBouncingBall(60,vNeo2),
                                    npBouncingBall(40,vNeo3),npBouncingBall(20,vNeo4)};

float accel[3] = {0,0,0};
byte *cb;
// in Gs, 1 is base
float Motion_Theshold = 2;

//Create instance of LSM6DS3Core
LSM6DS3 myIMU(I2C_MODE, 0x6A);    //I2C device address 0x6A

void setup_bounce() {
    //Call .beginCore() to configure the IMU
    if (myIMU.begin() != 0) {
        Serial.print("\nDevice Error.\n");
    } else {
        Serial.print("\nDevice OK.\n");
    }
}

void run_bounce() {
    float accel_sum;
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

    accel_sum = sqrt(accel[0]*accel[0] + accel[1]*accel[1] + accel[2]*accel[2]);

    for(int i=0;i<NSTRIPES;i++) bballs[i].update();

    if (bballs[3].hasFinished() && (accel_sum>Motion_Theshold))
    {
        cb=Wheel(random(0,255));

        for(int i=0;i<NSTRIPES;i++) 
        {
            //uncomment following line for fixed collor for the balls
            //and the upper line c=Wheel... 
            bballs[i].changeColor(*cb, *(cb+1), *(cb+2));
            bballs[i].restart();
        }
    }

    delay(5);
}
