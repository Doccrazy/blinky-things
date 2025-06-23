#include "src/npRunningLine.h"

#define LINE_SPEED 75

npRunningLine* rainbows[NSTRIPES_HOR] = {0};

void setup_line() {
    for (int i = 0; i < NSTRIPES_HOR; i++) {
        rainbows[i] = new npRunningLine(LINE_SPEED, *vNeoHor[i], i > NSTRIPES_HOR/2 ? NSTRIPES_HOR - i - 1 : i);
    }
}

void restart_line() {
    pixels.setBrightness(255);
    for(int i=0;i<NSTRIPES_HOR;i++) { 
        rainbows[i]->restart();
    }
}

void run_line() {
    bool update = false;

    for(int i=0;i<NSTRIPES_HOR;i++) { 
        bool didUpdate = rainbows[i]->update();
        update = update || didUpdate;

        if (rainbows[i]->hasFinished()) {
            rainbows[i]->restart();
        }
    };

    if (update)
    pixels.npShow();
}
