// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include <FastLED.h>
#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUM_LEDS       12
CRGB leds[NUM_LEDS];

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
/*Adafruit_NeoPixel pixels = Adafruit_NeoPixel(, PIN, NEO_GRB + NEO_KHZ800);*/

int delayval = 10; // delay for half a second
int lastI =  -1;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, 6>(leds, NUM_LEDS);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] *= 0;
  }
  FastLED.show();
  pinMode(13, INPUT);
}
int total = 0;

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

int i = 0;
int highTime = 0;
int prevHighTime = 0;
int fadeLength = 6;
int lastState = LOW;

void loop() {

  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  int  buttonState =  digitalRead(13);
  int time = millis();
  /*int timeSinceLastHighState = time - prevHighTime;*/

  if(buttonState == HIGH and lastState != HIGH){
    if (i == NUM_LEDS) {
      i = 0;
    }
    total +=1;
    leds[i].r = 255*sin(total*0.04);
    leds[i].b = 255*cos(total*0.04);
    leds[i].g = 255*sin(total*0.08);

    for (int shift = 0; shift < fadeLength; shift++) {
      int pixelJ = getPixelIndex(i, -shift);
      leds[pixelJ].nscale8( 128);
    }

    FastLED.show();

    i += 1;
    Serial.println(time);

    prevHighTime = time;


  }
  lastState = buttonState;
}
