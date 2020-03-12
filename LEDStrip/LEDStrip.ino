// led library
#include <FastLED.h>
#include <math.h>

// I2C stuff
#include <Wire.h>
#define SLAVE_ADDRESS 0x08

// led library definitions
#define LED_TYPE WS2812
#define NUM_LEDS 433
#define DATA_PIN 6
//CRGB leds[NUM_LEDS];

uint8_t max_bright = 255;

#include "MattsLedStrip.h"

MattsLedStrip *ptr;

void setup() {
  ptr = new MattsLedStrip(433);
  Serial.begin(9600);

  //FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(ptr->leds, 433);
  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 8000);
  FastLED.clear();

  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

}

void receiveData(int bytecount)
{
  byte register_num = 0;
  byte data_to_echo = 0;
  for (byte i = 0; i < bytecount; i++) {
    if(i == 0) register_num = Wire.read();
    else data_to_echo = Wire.read();
  }
  AdjustSettings(register_num,data_to_echo);
}

void AdjustSettings(int r,int val)
{
  switch (r)
  {
    case 0:
      //ledStrip.timerInterval = val;
      ptr->timerInterval = val;
      Serial.println("In Register 0 :");
      Serial.println(val);
      Serial.println("");
      break;
    case 1:
      /*ledStrip.mod = val;
      ledStrip.CalculateMaxBrightness();
      ledStrip.CalculateColor();*/
      ptr->mod = val;
      ptr->CalculateMaxBrightness();
      ptr->CalculateColor();
      Serial.println("In Register 1 :");
      Serial.println(val);
      Serial.println("");
      break;
    case 2:
      //ledStrip.program_number = val;
      ptr->program_number = val;
      Serial.println("In Register 2 :");
      Serial.println(val);
      Serial.println("");
      //ledStrip.ProgramSetup();
      ptr->ProgramSetup();
  }
}

void sendData()
{
  //Wire.write(data_to_echo);
}

int getSoundLevel()
{
  return analogRead(0);
}

void checkSoundLevel()
{
  int val = analogRead(0);
  Serial.print("Sound=");
  Serial.println(val,DEC);//print the sound value to serial
}

void loop() {
  //ledStrip.CheckTime();
  ptr->CheckTime();
  //checkSoundLevel();
}
