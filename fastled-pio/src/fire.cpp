#include "fire.h"

#include "constants.h"

#include <FastLED.h>

bool          gReverseDirection = false;
CRGBPalette16 gPal;

void setup_fire() {
  // Initialize the fire palette with a gradient from black to red to yellow to white
  gPal = HeatColors_p;
}

// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 55, suggested range 20-100
#define COOLING 15

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 80

#define FRAMES_PER_SECOND 30

void Fire2012WithPalette() {
  // Array of temperature readings at each simulation cell
  static uint8_t heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
  for (int i = 0; i < NUM_LEDS; i++) { heat[i] = qsub8(heat[i], random8(0, ((COOLING * 10) / NUM_LEDS) + 2)); }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for (int k = NUM_LEDS - 1; k >= 2; k--) { heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3; }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if (random8() < SPARKING) {
    int y   = random8(7);
    heat[y] = qadd8(heat[y], random8(160, 255));
  }

  // Step 4.  Map from heat cells to LED colors
  for (int j = 0; j < NUM_LEDS; j++) {
    // Scale the heat value from 0-255 down to 0-240
    // for best results with color palettes.
    uint8_t colorindex = scale8(heat[j], 240);
    CRGB    color      = ColorFromPalette(gPal, colorindex);
    int     pixelnumber;
    if (gReverseDirection) {
      pixelnumber = (NUM_LEDS - 1) - j;
    } else {
      pixelnumber = j;
    }
    leds[pixelnumber] = color;
  }
}

void run_fire() {
  random16_add_entropy(random());

  Fire2012WithPalette(); // run simulation frame, using palette colors

  FastLED.show();        // display this frame
  delay(1000 / FRAMES_PER_SECOND);
}
