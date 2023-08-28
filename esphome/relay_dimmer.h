#include "esphome.h"
using namespace esphome;
const int ledCount = 6;    // the number of LEDs in the bar graph
//int ledPins[] = {14, 12, 13, 4, 2, 16};   // an array of pin numbers to which LEDs are attached
int ledPins[] = {14, 4, 12, 2, 13, 16};   // an array of pin numbers to which LEDs are attached
int offset = 0;
int lastLedLevel = 0;

class MyCustomFloatOutput : public Component, public FloatOutput {
 public:
  void setup() override {
    // This will be called by App.setup()
    for (int thisLed = 0; thisLed < ledCount; thisLed++) {  // loop over the pin array and set them all to output:
      pinMode(ledPins[thisLed], OUTPUT);
    }
    pinMode(15, OUTPUT);
    //digitalWrite(15, HIGH);
  }

  void write_state(float state) override {
    // state is the amount this output should be on, from 0.0 to 1.0
    // we need to convert it to an integer first
    id(dimmer_lvl) = state;
    int value = state * 1024;
  
    int ledLevel = map(value, 0, 1024, 0, ledCount);    // map the result to a range from 0 to the number of LEDs:
    
    if (ledLevel != lastLedLevel) {
    lastLedLevel = ledLevel;
      //ESP_LOGD("main", "LED Level %d", ledLevel);
      if (ledLevel == 0) {
        digitalWrite(15, LOW);
        offset++;
        //ESP_LOGD("main", "offset Level %d", offset);
        if (offset == ledCount) {
          offset = 0;
        }
      } else {
        digitalWrite(15, HIGH);
      }
    }

    for (int thisLed = 0 + offset; thisLed < ledCount + offset; thisLed++) {  // loop over the LED array:
      if (thisLed < ledLevel + offset) { // if the array element's index is less than ledLevel, turn the pin for this element on:
        digitalWrite(ledPins[thisLed % ledCount], HIGH);
      }
      else {    // turn off all pins higher than the ledLevel:
        digitalWrite(ledPins[thisLed % ledCount], LOW);
      }
    }
  }
};