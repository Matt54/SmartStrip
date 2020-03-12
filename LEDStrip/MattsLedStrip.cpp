#include "MattsLedStrip.h"
#include <math.h>

MattsLedStrip::MattsLedStrip(int num){
  leds[num];
  CalculateMaxBrightness();
  CalculateColor();
  timeNow = millis();
  timePast = millis();
  numLEDS = num;
}

/*
This is specific to my Led strip which has A LOT OF LedS (433)
  - I use this to limit the brightness when there are lots of them on,
    otherwise there are glitches that occur in the strip
  - I determined the function through testing and curve fitting
*/
void MattsLedStrip::CalculateMaxBrightness(){
  double y = 3.3972 * pow(mod,2) - 8.9219 * mod + 38.476;
  if(y > 255) y = 255;
  maxB = round(y);
}

/* This is to get the real color values when maxB is not at 255 */
void MattsLedStrip::CalculateColor(){
  rgb[0] = maxB * ((float)relativeRGB[0] / 255);
  rgb[1] = maxB * ((float)relativeRGB[1] / 255);
  rgb[2] = maxB * ((float)relativeRGB[2] / 255);
}

/*
CheckTime gate keeps the stepping functions so that they only occur
at the rate of time timeInterval variable
*/
void MattsLedStrip::CheckTime(){
  timeNow = millis();
  if( (timeNow - timePast) > timerInterval){
    timePast = timeNow;
    StepStrip();
  }
}

void MattsLedStrip::StepStrip(){
  switch(program_number){
    case 1:
      ModStep(true);
      break;
    case 2:
      ModStep(false);
      break;
    case 3:
      PixelRun();
      break;
    case 4:
      AllOn();
      break;
  }
}

void MattsLedStrip::ProgramSetup(){
  if(program_number == 4)
  {
    mod = 1;
    CalculateColor();
    CalculateMaxBrightness();
  }
}

/*
ModStep is a stepping function that spaces LedS according to a modulus
variable. The spaced LedS move from beginning to end and then restart.
INPUT: hasTail (bool) - creates a tail after the LedS (meteor effect)
*/
void MattsLedStrip::ModStep(bool hasTail){
  if (autoColor) ChangeColor();
  FastLED.clear();
  for(int i = 0; i < numLEDS - counter; i++)
  {
    if(i % mod == 0)
    {
      // Always write the main Led
      leds[i + counter] = CRGB( rgb[0], rgb[1], rgb[2]);

      /*
      If hasTail, we need to loop through the rest of the LedS between
      our main Led and the previous Led and light those to dimmer values
      that we calculate in WriteLedTrail
      */
      if(hasTail)
      {
        for(int j = 1; j < mod; j = j + 1) WriteLedTrail(i + counter - j, j);
      }
    }
  }
  FastLED.show();
  counter++;
  if(counter > mod - 1) counter = 0;
}

void MattsLedStrip::WriteLedTrail(int ledNum, byte dist){
  byte v1 = (float) rgb[0] / (dist + 1);
  byte v2 = (float) rgb[1] / (dist + 1);
  byte v3 = (float) rgb[2] / (dist + 1);
  if (ledNum >= 0) leds[ledNum] = CRGB( v1, v2, v3);
}

/*
PixelRun is a stepping function that has one Led pixel move through the
light strip from beginning to end, and then back.
*/
void MattsLedStrip::PixelRun(){
  if (autoColor) ChangeColor();
  leds[counter] = CRGB( rgb[0], rgb[1], rgb[2]);
  FastLED.show();
  leds[counter] = CRGB( 0, 0, 0);
  BounceCount();
}

void MattsLedStrip::BounceCount(){
  if(!isBackwards)
  {
    counter++;
    if(counter > numLEDS)
    {
      counter--;
      isBackwards = true;
    }
  }
  else
  {
    counter--;
    if(counter < 0)
    {
      counter++;
      isBackwards = false;
    }
  }
}

void MattsLedStrip::AllOn(){
  if(getSoundLevel() > 300)
  {
    ResetColor();
    for(int i = 0; i < numLEDS; i++) leds[i] = CRGB( rgb[0], rgb[1], rgb[2]);
    FastLED.show();
    isFading = true;
    thresholdCount++;
    if(thresholdCount > 16) RandomColor();
  }
  else if(!isFading) ClearStrip();
  else
  {
    for(int i = 0; i < numLEDS; i++) leds[i] = CRGB( rgb[0], rgb[1], rgb[2]);
    FadeOut();
    FastLED.show();
  }
}

void MattsLedStrip::FadeOut(){
  if(rgb[0] > 0) rgb[0] = rgb[0] - 1;
  if(rgb[1] > 0) rgb[1] = rgb[1] - 1;
  if(rgb[2] > 0) rgb[2] = rgb[2] - 1;
  if( (rgb[0] == 0) && (rgb[1] == 0) && (rgb[2] == 0) )
  {
    isFading = false;
    ResetColor();
  }
}

void MattsLedStrip::RandomColor(){
  relativeRGB[0] = random(0, 255);
  relativeRGB[1] = random(0, 255 - relativeRGB[0]);
  relativeRGB[2] = random(0, 255 - relativeRGB[1] - relativeRGB[0]);
  CalculateColor();
  thresholdCount = 0;
}

void MattsLedStrip::ResetColor(){
  CalculateColor();
}

void MattsLedStrip::ChangeColor(){
  if( (rgb[0] > 0) && !isTurnaround)
  {
    rgb[0] = rgb[0] - 1;
    rgb[1] = rgb[1] + 1;
  }
  else if(rgb[1] > 0)
  {
    rgb[1] = rgb[1] - 1;
    rgb[2] = rgb[2] + 1;
  }
  else if(rgb[2] > 0)
  {
    rgb[2] = rgb[2] - 1;
    rgb[0] = rgb[0] + 1;
    isTurnaround = true;
    if(rgb[2]==0)
    {
      isTurnaround = false;
    }
  }
}

void MattsLedStrip::ClearStrip(){
  FastLED.clear();
  FastLED.show();
}

int MattsLedStrip::getSoundLevel()
{
  return analogRead(0);
}
