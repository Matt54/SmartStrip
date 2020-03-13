#include <FastLED.h>
#include <arduino.h>

class MattsLedStrip{
private:
public:
 unsigned long timeNow;
 unsigned long timePast;
 int timerInterval = 10;

 int counter = 0; // needs to be an int because it can go up to NUM_LEDS

 CRGB leds[433];

 CHSV chsv; // Color (Hue, Saturation, Value)
 // Hue - color value from 0 - 255
 // Saturation - Colorfulness
 // Value - brightness

 uint8_t maxB = 0;
 uint8_t program_number = 1;
 uint8_t mod = 25;
 bool isBackwards = false;
 bool autoColor = true;

 bool isFading = false;
 uint8_t thresholdCount = 0;
 int numLEDS;

 MattsLedStrip(int num);
 void CalculateMaxBrightness();
 void CheckTime();
 void StepStrip();
 void ProgramSetup();
 void ModStep(bool hasTail);
 void WriteLedTrail(int ledNum, byte dist);
 void PixelRun();
 void BounceCount();
 void AllOn();
 void FadeOut();
 void Wave();
 void RandomColor();
 void ResetColor();
 void ChangeHue();
 void ClearStrip();
 int getSoundLevel();

};
