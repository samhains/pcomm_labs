// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include <FastLED.h>

#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUM_LEDS       12
#define FADE_LENGTH 6
CRGB leds[NUM_LEDS];

int i = 0;
int lastState = LOW;
int total = 0;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, 6>(leds, NUM_LEDS);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] *= 0;
  }
  FastLED.show();
  pinMode(13, INPUT);
}

int getPixelIndex(int current, int shift) {
  int next = current + shift;
  int gap;
  if (next >= NUM_LEDS) {
    gap = next - (NUM_LEDS-1);
    return gap;
  }  else if (next < 0) {
    return NUM_LEDS+next;
  } else {
    return current + shift;
  }

}

void loop() {

  int  buttonState =  digitalRead(13);
  int time = millis();

  if(buttonState == HIGH and lastState != HIGH){
    if (i == NUM_LEDS) {
      i = 0;
    }
    total +=1;
    leds[i].r = 255*sin(total*0.04);
    leds[i].b = 255*cos(total*0.04);
    leds[i].g = 255*sin(total*0.08);

    for (int shift = 0; shift < FADE_LENGTH; shift++) {
      int pixelJ = getPixelIndex(i, -shift);
      leds[pixelJ].nscale8( 128);
    }

    FastLED.show();

    i += 1;
    Serial.println(time);



  }
  lastState = buttonState;
}
