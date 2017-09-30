// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include <FastLED.h>

// How many NeoPixels are attached to the Arduino?
#define NUM_LEDS       12
#define NUM_NOTES 57
#define FADE_LENGTH 6
#define FADE_SCALE 128
#define SPEAKER_PIN 8
#define FIDGET_PIN 15
#define NEOPIXEL_PIN 10
#include "pitches.h"


int melody[] ={ NOTE_G3, NOTE_C4, NOTE_C4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_G4, NOTE_E4, NOTE_C4, NOTE_G3, NOTE_C4 , NOTE_C4, NOTE_D4, NOTE_F4, NOTE_E4, NOTE_C4, NOTE_G3, NOTE_C4, NOTE_C4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_G4, NOTE_E4, NOTE_C4,  NOTE_A5, NOTE_D4, NOTE_F4, NOTE_E4, NOTE_C4, NOTE_G4, NOTE_C5, NOTE_C5,NOTE_A5, NOTE_C5, NOTE_B5, NOTE_D5, NOTE_B5, NOTE_G4, NOTE_G4,NOTE_C5, NOTE_C5, NOTE_A5, NOTE_C5, NOTE_B5, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A5, NOTE_B5, NOTE_C5, NOTE_A5,  NOTE_D4, NOTE_F4, NOTE_E4, NOTE_C4};

int noteDurations[] = { 8,  4,  8,  4,  8,  8,  8,  8,  4,  8,  4,   8,  4,  8, 3, 4, 8, 4, 8,   4,  8,  8,  8,  8,  3,  3, 4, 8, 3, 8, 4, 8, 4, 8, 8, 8,  8, 4, 8, 4, 8,  4, 8, 3, 3, 4, 8,  4,  8, 4, 8, 3, 3, 4, 8, 3, 4 };


CRGB leds[NUM_LEDS];

int currentLedIndex = 0;
int lastState = LOW;
int total = 0;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, NEOPIXEL_PIN>(leds, NUM_LEDS);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] *= 0;
  }
  FastLED.show();
  pinMode(FIDGET_PIN, INPUT);
  for (int thisNote = 0; thisNote < NUM_NOTES; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(SPEAKER_PIN, melody[thisNote], noteDuration);

    //pause for the note's duration plus 30 ms:
    delay(noteDuration + 30);
  }
}

int getPixelIndex(int current, int shift) {
  int next = current + shift;
  int gap;
  if (next >= NUM_LEDS) {
    gap = next - (NUM_LEDS - 1);
    return gap;
  }  else if (next < 0) {
    return NUM_LEDS + next;
  } else {
    return current + shift;
  }

}


int getNoteIndex(int current, int shift) {
  int next = current + shift;
  int gap;
  if (next >= NUM_NOTES) {
    gap = next - (NUM_NOTES - 1);
    return gap;
  }  else if (next < 0) {
    return NUM_NOTES + next;
  } else {
    return current + shift;
  }
}

void loop() {

  int  buttonState =  digitalRead(FIDGET_PIN);
  Serial.println(buttonState);
  int time = millis();

  if (buttonState == HIGH and lastState != HIGH) {
    if (currentLedIndex == NUM_LEDS) {
      currentLedIndex = 0;
    }

    int noteIndex = getNoteIndex(currentLedIndex, 1);
    int noteDuration = 1000 / noteDurations[noteIndex];
    tone(SPEAKER_PIN, melody[noteIndex], noteDuration);


    total += 1;
    leds[currentLedIndex].r = 255 * sin(total * 0.04);
    leds[currentLedIndex].b = 255 * cos(total * 0.04);
    leds[currentLedIndex].g = 255 * sin(total * 0.08);

    for (int shift = 0; shift < FADE_LENGTH; shift++) {
      int pixelFadeIndex = getPixelIndex(currentLedIndex, -shift);
      leds[pixelFadeIndex].nscale8( FADE_SCALE );
    }

    FastLED.show();

    currentLedIndex += 1;



  }
  lastState = buttonState;
}

