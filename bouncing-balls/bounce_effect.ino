#include <npNeoPixel.h>
#include "src/npBouncingBallCustom.h"

npBouncingBallCustom* bballs[NSTRIPES] = {0};

byte *cb;
// in Gs, 1 is base
float Motion_Theshold_Sq = pow(2.75, 2);

void setup_bounce() {
    for (int i = 0; i < NSTRIPES; i++) {
        bballs[i] = new npBouncingBallCustom(random(10, 80), *vNeoVert[i], false);
    }
}

void restart_bounce() {
    pixels.setBrightness(255);
    for(int i=0;i<NSTRIPES;i++) { 
        bballs[i]->restart();
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
        bool didUpdate = bballs[i]->update();
        update = update || didUpdate;
    };

    if (bballs[1]->hasFinished() && (accel_sum_sq>Motion_Theshold_Sq))
    {
        cb=Wheel(random(0,255));

        for(int i=0;i<NSTRIPES;i++) 
        {
            //uncomment following line for fixed collor for the balls
            //and the upper line c=Wheel... 
            bballs[i]->changeColor(*cb, *(cb+1), *(cb+2));
            bballs[i]->restart();
        }
    }

    if (update)
    pixels.npShow();

    //delay(5);
}
