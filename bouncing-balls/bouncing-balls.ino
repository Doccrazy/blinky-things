#include "LSM6DS3.h"
#include "Wire.h"
#include "SPI.h"
#include <Adafruit_NeoPixel.h>
#include <npNeoPixel.h>
#include <npVirtualNeo.h>

#define NeoPin     A0 // NeoPixel Pin
#define MAXPIXELS 52 // Number of Pixels
#define NSTRIPES 4 //Number of Stripes 

npNeoPixel pixels = npNeoPixel(MAXPIXELS, NeoPin, NEO_GRB + NEO_KHZ800, 8.0);

npVirtualNeo vNeo1(&pixels, 12, 0);
npVirtualNeo vNeo2(&pixels, 13, 25);
npVirtualNeo vNeo3(&pixels, 38, 26);
npVirtualNeo vNeo4(&pixels, 39, 51);

uint effectId = 0;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    pixels.begin();
    pixels.clear();
    pixels.npShow();

    //Init Serial port
    Serial.begin(9600);
    while (!Serial);

    setup_bounce();
    setup_bluetooth();
}

void loop() {
    if (effectId == 0) {
        run_bounce();
    } else if (effectId == 1) {
        run_meteor();
    }
}

void bleuart_receive(char* str) {
    effectId = (effectId+1)%2;

    pixels.clear();
    pixels.npShow();
}
