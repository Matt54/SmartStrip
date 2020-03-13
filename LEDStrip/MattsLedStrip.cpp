#include "MattsLedStrip.h"
#include <math.h>

MattsLedStrip::MattsLedStrip(int num){
  leds[num];
  CalculateMaxBrightness();
  timeNow = millis();
  timePast = millis();
  numLEDS = num;
  chsv.hue = 0;
  chsv.saturation = 255;
}

/*
This is specific to my Led strip which has A LOT of Leds (433)
  - I use this to limit the brightness when there are lots of them on,
    otherwise there are glitches that occur in the strip
  - I determined the function through testing and curve fitting
*/
void MattsLedStrip::CalculateMaxBrightness(){
  double y = 3.3972 * pow(mod,2) - 8.9219 * mod + 38.476;
  if(y > 255) y = 255;
  maxB = round(y);
  chsv.value = maxB;
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
    case 5:
      mod = 6;
      CalculateMaxBrightness();
      Wave();
      break;
  }
}

void MattsLedStrip::ProgramSetup(){
  if(program_number == 4)
  {
    mod = 6;
    //CalculateColor();
    CalculateMaxBrightness();
  }
}

/*
ModStep is a stepping function that spaces LedS according to a modulus
variable. The spaced LedS move from beginning to end and then restart.
INPUT: hasTail (bool) - creates a tail after the LedS (meteor effect)
*/
void MattsLedStrip::ModStep(bool hasTail){
  //if (autoColor) ChangeColor();
  if (autoColor) ChangeHue();
  FastLED.clear();
  for(int i = 0; i < numLEDS - counter; i++)
  {
    if(i % mod == 0)
    {
      // Always write the main Led
      //leds[i + counter] = CRGB( rgb[0], rgb[1], rgb[2]);

      chsv.value = maxB;
      hsv2rgb_rainbow(chsv, leds[i + counter]);
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
  chsv.value = (float) maxB / (dist + 1);
  if (ledNum >= 0) hsv2rgb_rainbow(chsv, leds[ledNum]);
}

/*
PixelRun is a stepping function that has one Led pixel move through the
light strip from beginning to end, and then back.
*/
void MattsLedStrip::PixelRun(){
  if (autoColor) ChangeHue();
  chsv.value = maxB;
  hsv2rgb_rainbow(chsv, leds[counter]);
  FastLED.show();
  chsv.value = 0;
  hsv2rgb_rainbow(chsv, leds[counter]);
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
    for(int i = 0; i < numLEDS; i++) hsv2rgb_rainbow(chsv, leds[i]);
    FastLED.show();
    isFading = true;
    thresholdCount++;
    if(thresholdCount > 16) RandomColor();
  }
  else if(!isFading) ClearStrip();
  else
  {
    for(int i = 0; i < numLEDS; i++) hsv2rgb_rainbow(chsv, leds[i]);
    FadeOut();
    FastLED.show();
  }
}

void MattsLedStrip::FadeOut(){
  chsv.value = chsv.value - 1;
  if(chsv.value == 0){
    isFading = false;
    ResetColor();
  }
  
}

void MattsLedStrip::Wave(){
  counter++;
  if(counter >= numLEDS) counter = 0;
  for(int i = 0; i < numLEDS; i++)
  {
      chsv.hue = map(i+counter, 0, numLEDS, 0, 255);
      hsv2rgb_rainbow(chsv, leds[i]);
  }
  FastLED.show();
}

void MattsLedStrip::RandomColor(){
  chsv.hue = random8(0,255);
  thresholdCount = 0;
}

void MattsLedStrip::ResetColor(){
  chsv.value = maxB;
}

void MattsLedStrip::ChangeHue(){
   if(chsv.hue < 255) chsv.hue = chsv.hue + 1;
   else chsv.hue = 0;
}

void MattsLedStrip::ClearStrip(){
  FastLED.clear();
  FastLED.show();
}

int MattsLedStrip::getSoundLevel()
{
  return analogRead(0);
}
