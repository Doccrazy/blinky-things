#include <vector>
#include "LSM6DS3.h"
#include "Wire.h"
#include "SPI.h"
// #include <Adafruit_NeoPixel.h>
// #include <npNeoPixel.h>
// #include <npVirtualNeo.h>
#include <FastLED.h>

#define NeoPin     A0 // NeoPixel Pin
#define SIDE_LEN 40
#define NSTRIPS 12 //Number of Strips 
#define MAXPIXELS SIDE_LEN*NSTRIPS // Number of Pixels

// npNeoPixel pixels = npNeoPixel(MAXPIXELS, NeoPin, NEO_GRB + NEO_KHZ800, 8.0);

// std::vector<npVirtualNeo> virtualStrips;

//Create instance of LSM6DS3Core
LSM6DS3 myIMU(I2C_MODE, 0x6A);    //I2C device address 0x6A

uint effectId = 0;

#define NUM_LEDS SIDE_LEN*NSTRIPS
#define DATA_PIN 3
#define BRIGHTNESS 64
CRGB leds[NUM_LEDS];

void setup() {
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    //    .setCorrection(TypicalLEDStrip)
    //    .setDither(BRIGHTNESS < 255);

  // set master brightness control
  //FastLED.setBrightness(BRIGHTNESS);

    // for (int i = 0; i < NSTRIPS; i++) {
    //     virtualStrips.push_back(npVirtualNeo(&pixels, i*SIDE_LEN, (i+1)*SIDE_LEN - 1));
    // }

    pinMode(LED_BUILTIN, OUTPUT);

    // pixels.begin();
    // pixels.clear();
    // pixels.npShow();

    //Init Serial port
    Serial.begin(9600);
    while (!Serial);

    //Call .beginCore() to configure the IMU
    if (myIMU.begin() != 0) {
        Serial.print("\nDevice Error.\n");
    } else {
        Serial.print("\nDevice OK.\n");
    }

    setup_bluetooth();
}

void loop() {
    // if (effectId == 0) {
    //     run_sparkle();
    // } else if (effectId == 1) {
    //     run_meteor();
    // }
    // pixels.npShow();
  //pride();
    //  FastLED.show();  
    leds[0] = CRGB::Red; FastLED.show(); delay(300);
    leds[0] = CRGB::Black; FastLED.show(); delay(300);
}


// This function draws rainbows with an ever-changing,
// widely-varying set of parameters.
void pride() 
{
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
