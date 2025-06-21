#include <npSparkle.h>

int setupDone = 0;

std::vector<npBase*> sparkles;

class npSparkle2 : public npSparkle {
  public:
    npSparkle2(unsigned char red, unsigned char green, unsigned char blue, unsigned int SpeedDelay, npVirtualNeo Target, bool KeepBackground = false) : npSparkle(red, green, blue, SpeedDelay, Target, KeepBackground)
    {
      autoUpdate = false;
    }
    //-------------------------------------------------------------------------------------
    npSparkle2(unsigned int SpeedDelay, npVirtualNeo Target, bool KeepBackground = false) : npSparkle(SpeedDelay, Target, KeepBackground)
    {
      autoUpdate = false;
    }
    //-------------------------------------------------------------------------------------
    ~npSparkle2()
    {
    }
};

void setup_sparkle() {
  for (int i = 0; i < NSTRIPS; i++) {
    sparkles.push_back(new npSparkle2(255, 255, 255, random(50, 250), virtualStrips[i]));
  }
  Serial.print("\nSparkles setup done.\n");
  setupDone = 1;
}

int count = 0;

void run_sparkle() {
  if (!setupDone) {
    setup_sparkle();
  }

  for (int i = 0; i < NSTRIPS; i++) {
    sparkles[i]->update();
    if (sparkles[i]->hasFinished()) sparkles[i]->restart();   
  }
  Serial.printf("update %d\n", count);
  count++;

  delay(5);
}
