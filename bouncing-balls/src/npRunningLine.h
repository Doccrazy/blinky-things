// Neopixel effect using npBase, npNeoPixel and npVirtualNeo classes.
// Based on the ideas from:
// https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/ 
//
#ifndef NPRUNNINGLINE_H
#define NPRUNNINGLINE_H

#include <npBase.h>
//-------------------------------------------------------------------------------------
class npRunningLine : public npBase {
public:
  npRunningLine(unsigned char red, unsigned char green, unsigned char blue, unsigned int SpeedDelay, npVirtualNeo Target, bool KeepBackground = false, bool AutoUpdate = true);
  npRunningLine(unsigned int SpeedDelay, npVirtualNeo Target, unsigned int Offset, bool Mirror = false);
  ~npRunningLine();
private:
  void doInit();
  bool doUpdate();
  unsigned int lastPosition;
  uint32_t oldColor;
  uint32_t offset;
  bool mirror;
};
//-------------------------------------------------------------------------------------

#endif // NPRUNNINGLINE_H
