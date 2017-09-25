// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include <FastLED.h>

#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUM_LEDS       12
#define FADE_LENGTH 6
#define FADE_SCALE 128

CRGB leds[NUM_LEDS];

int currentLedIndex = 0;
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
    if (currentLedIndex == NUM_LEDS) {
      currentLedIndex = 0;
    }
    total +=1;
    leds[currentLedIndex].r = 255*sin(total*0.04);
    leds[currentLedIndex].b = 255*cos(total*0.04);
    leds[currentLedIndex].g = 255*sin(total*0.08);

    for (int shift = 0; shift < FADE_LENGTH; shift++) {
      int pixelFadeIndex = getPixelIndex(currentLedIndex, -shift);
      leds[pixelFadeIndex].nscale8( FADE_SCALE );
    }

    FastLED.show();

    currentLedIndex += 1;
    Serial.println(time);



  }
  lastState = buttonState;
}
