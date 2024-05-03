/*
 * Author Alex Waine
 *
 * Esp8266 FastLED
 *
*  Version 0.9.0
 *
*/

#ifndef Section_h
#define Section_h




struct Section {
  String name;
  int dataPin;
  int ledLength;
  uint8_t hue;
  String effect;
  String currentEffect;
  int brightness = 50;
  int rgb[3];
  int currentRgb[3];
  int hsv[3];
  int millis;
  CRGB* ledArr;
  int currentLed;
};


#endif