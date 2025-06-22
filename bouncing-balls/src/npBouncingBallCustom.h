// Neopixel effect using npNeoPixel and npVirtualNeo classes.
// Based on the ideas from:
// https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/ 
//
#ifndef NPBOUNCINGBALLSCUSTOM_H
#define NPBOUNCINGBALLSCUSTOM_H

#include <math.h>
#include <npBase.h>
//-------------------------------------------------------------------------------------
class npBouncingBallCustom : public npBase {
public:
  npBouncingBallCustom(unsigned char red, unsigned char green, unsigned char blue, unsigned int SpeedDelay, npVirtualNeo Target, bool AutoUpdate);
  npBouncingBallCustom(unsigned int SpeedDelay, npVirtualNeo Target, bool AutoUpdate);
  ~npBouncingBallCustom();
private:
  void doInit();
  bool doUpdate();
  float Gravity;
  unsigned int StartHeight;
  float Height;
  float ImpactVelocityStart;
  float ImpactVelocity;
  float Dampening;
  uint32_t ClockTimeSinceLastBounce;
  uint32_t TimeSinceLastBounce;
  unsigned int lastPosition;
};
//-------------------------------------------------------------------------------------

#endif // NPBOUNCINGBALLSCUSTOM_H
