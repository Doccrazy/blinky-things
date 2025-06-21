#define FASTLED_NRF52_SUPPRESS_UNTESTED_BOARD_WARNING 1
#define FASTLED_OVERCLOCK 1.2
//#define FASTLED_FORCE_SOFTWARE_PINS 1
#include <vector>
#include "Wire.h"
#include "SPI.h"
#include <FastLED.h>

#define SIDE_LEN 40
#define NSTRIPS 12 //Number of Strips 

uint effectId = 0;

#define NUM_LEDS 216
#define DATA_PIN 3
#define BRIGHTNESS 16
CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  pinMode(LED_BUILTIN, OUTPUT);

  //Init Serial port
  Serial.begin(115200);
  while (!Serial) {delay(10);}

  setup_imu();
  setup_bluetooth();
}

void loop() {
  // if (effectId == 0) {
  //     run_sparkle();
  // } else if (effectId == 1) {
  //     run_meteor();
  // }
  pride();
  FastLED.show();  
  // leds[0] = CRGB::Red; FastLED.show(); delay(300);
  // leds[0] = CRGB::Black; FastLED.show(); delay(300);
}

// This function draws rainbows with an ever-changing,
// widely-varying set of parameters.
void pride() {
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;
 
  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);
  
  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;
  
  for( uint16_t i = 0 ; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);
    
    CRGB newcolor = CHSV( hue8, sat8, bri8);
    
    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS-1) - pixelnumber;
    
    nblend( leds[pixelnumber], newcolor, 64);
  }
}

void bleuart_receive(char* str) {
    effectId = (effectId+1)%2;

    // pixels.clear();
    // pixels.npShow();
}
