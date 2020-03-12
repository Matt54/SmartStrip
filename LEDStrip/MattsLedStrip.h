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

 byte rgb[3] = { 0 , 0 , 0 }; // main array for storing color values

 // This will be used to adjust the color values manually
 byte relativeRGB[3] = {255 ,0 ,0  };

 byte maxB = 0;
 byte program_number = 1;
 byte mod = 25;
 bool isTurnaround = false;
 bool isBackwards = false;
 bool autoColor = true;

 bool isFading = false;
 byte thresholdCount = 0;
 int numLEDS;

 MattsLedStrip(int num);
 void CalculateMaxBrightness();
 void CalculateColor();
 void CheckTime();
 void StepStrip();
 void ProgramSetup();
 void ModStep(bool hasTail);
 void WriteLedTrail(int ledNum, byte dist);
 void PixelRun();
 void BounceCount();
 void AllOn();
 void FadeOut();
 void RandomColor();
 void ResetColor();
 void ChangeColor();
 void ClearStrip();
 int getSoundLevel();
};
