#include "src/npRunningLine.h"

#define LINE_SPEED 75

npRunningLine rainbows[NSTRIPES_HOR] = {
    npRunningLine(LINE_SPEED, vNeoHor1, 0),
    npRunningLine(LINE_SPEED, vNeoHor2, 1),
    npRunningLine(LINE_SPEED, vNeoHor3, 2),
    npRunningLine(LINE_SPEED, vNeoHor4, 3),
    npRunningLine(LINE_SPEED, vNeoHor5, 4),
    npRunningLine(LINE_SPEED, vNeoHor6, 5),
    npRunningLine(LINE_SPEED, vNeoHor7, 6),
    npRunningLine(LINE_SPEED, vNeoHor8, 7),
    npRunningLine(LINE_SPEED, vNeoHor9, 6),
    npRunningLine(LINE_SPEED, vNeoHor10, 5),
    npRunningLine(LINE_SPEED, vNeoHor11, 4),
    npRunningLine(LINE_SPEED, vNeoHor12, 3),
    npRunningLine(LINE_SPEED, vNeoHor13, 2),
    npRunningLine(LINE_SPEED, vNeoHor14, 1),
    npRunningLine(LINE_SPEED, vNeoHor15, 0)
};

void restart_line() {
    pixels.setBrightness(255);
    for(int i=0;i<NSTRIPES_HOR;i++) { 
        rainbows[i].restart();
    }
}

void run_line() {
    bool update = false;

    for(int i=0;i<NSTRIPES_HOR;i++) { 
        bool didUpdate = rainbows[i].update();
        update = update || didUpdate;

        if (rainbows[i].hasFinished()) {
            rainbows[i].restart();
        }
    };

    if (update)
    pixels.npShow();
}
