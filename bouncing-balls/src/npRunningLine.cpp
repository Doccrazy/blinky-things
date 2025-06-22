// Neopixel effect using npBase, npNeoPixel and npVirtualNeo classes.
// Based on the ideas from:
// https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/ 
//
#include "npRunningLine.h"
//-------------------------------------------------------------------------------------
npRunningLine::npRunningLine(unsigned char red, unsigned char green, unsigned char blue,
                           unsigned int SpeedDelay, npVirtualNeo Target, bool KeepBackground, bool AutoUpdate) :
                           npBase(Target, SpeedDelay, red, green, blue, KeepBackground, AutoUpdate)
{
  init();
  lastPosition = 0;
  oldColor = 0;
}
//-------------------------------------------------------------------------------------
npRunningLine::npRunningLine(unsigned int SpeedDelay, npVirtualNeo Target, unsigned int Offset, bool Mirror) :
                           npBase(Target, SpeedDelay, false, false)
{
  init();
  lastPosition = 0;
  oldColor = 0;
  offset = Offset;
  mirror = Mirror;
}
//-------------------------------------------------------------------------------------
npRunningLine::~npRunningLine()
{
}
//-------------------------------------------------------------------------------------
void npRunningLine::doInit()
{
}
//-------------------------------------------------------------------------------------
bool npRunningLine::doUpdate()
{
  int rc = false;
  if (doRainbow) {
    Wheel(((Position * 256 / target->numPixels()) + Position) & 255, &r, &g, &b);
  }
  if (keepBackground) {
    setPixelColor(lastPosition, oldColor);
    oldColor = getPixelColor(Position);
  }
  else {
    setPixelColor((lastPosition + offset + target->numPixels()) % target->numPixels(), 0, 0, 0);
    if (mirror)
    setPixelColor((target->numPixels() - (lastPosition + offset) + target->numPixels()) % target->numPixels(), 0, 0, 0);
  }
  rc = setPixelColor((Position + offset + target->numPixels()) % target->numPixels(), r, g, b);
  if (mirror)
  setPixelColor((target->numPixels() - (Position + offset) + target->numPixels()) % target->numPixels(), r, g, b);
  lastPosition = Position;
  Position++;
  if (Position >= target->numPixels()) {
    done = true;
    Position = 0;
  }
  return rc;
}
//-------------------------------------------------------------------------------------
