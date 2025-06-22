#include <npNeoPixel.h>
#include "src/npBouncingBallCustom.h"

npBouncingBallCustom bballs[NSTRIPES] = {
    npBouncingBallCustom(50, vNeo1, false),
    npBouncingBallCustom(10, vNeo2, false),
    npBouncingBallCustom(40, vNeo3, false),
    npBouncingBallCustom(20, vNeo4, false),
    npBouncingBallCustom(30, vNeo5, false),
    npBouncingBallCustom(80, vNeo6, false),
    npBouncingBallCustom(20, vNeo7, false),
    npBouncingBallCustom(10, vNeo8, false),
    npBouncingBallCustom(70, vNeo9, false),
    npBouncingBallCustom(30, vNeo10, false),
    npBouncingBallCustom(10, vNeo11, false),
    npBouncingBallCustom(40, vNeo12, false),
    npBouncingBallCustom(80, vNeo13, false),
    npBouncingBallCustom(10, vNeo14, false),
    npBouncingBallCustom(20, vNeo15, false),
    npBouncingBallCustom(60, vNeo16, false),
    npBouncingBallCustom(20, vNeo17, false),
    npBouncingBallCustom(50, vNeo18, false),
    npBouncingBallCustom(30, vNeo19, false),
};

byte *cb;
// in Gs, 1 is base
float Motion_Theshold_Sq = pow(2.75, 2);

void restart_bounce() {
    pixels.setBrightness(255);
    for(int i=0;i<NSTRIPES;i++) { 
        bballs[i].restart();
    }
}

void run_bounce() {
    bool update = false;
    //   // turn the LED on (HIGH is the voltage level)
    // digitalWrite(LED_BUILTIN, HIGH);
    // // // wait for a second
    // delay(25);
    // // // turn the LED off by making the voltage LOW
    // digitalWrite(LED_BUILTIN, LOW);

    // delay(25);

    for(int i=0;i<NSTRIPES;i++) { 
        bool didUpdate = bballs[i].update();
        update = update || didUpdate;
    };

    if (bballs[1].hasFinished() && (accel_sum_sq>Motion_Theshold_Sq))
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

    if (update)
    pixels.npShow();

    //delay(5);
}
