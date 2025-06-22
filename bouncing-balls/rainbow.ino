#include <npRainbow.h>

npRainbow rainbow(10, vNeoFull, true);

uint brightness = 0;

#define MAX_RAINBOW_BRIGHTNESS 64

void restart_rainbow() {
  pixels.setBrightness(0);
  brightness = 0;
}

void run_rainbow() {
  if (rainbow.update()) {
    brightness = min(brightness + 1, MAX_RAINBOW_BRIGHTNESS);
    pixels.setBrightness(brightness);
  }
  if (rainbow.hasFinished()) {
    rainbow.restart();
  }
}
