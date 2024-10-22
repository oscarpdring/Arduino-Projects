#ifndef STATUSLED_H
#define STATUSLED_H

#include <Adafruit_NeoPixel.h>
#include "ColorConverter.h"

class StatusLED {
private:
  Adafruit_NeoPixel ledStrip;
  uint16_t totalPixels;

  uint16_t progress;
  uint16_t updateIntervalMillis;    // Interval in milliseconds between animation updates
  unsigned long lastAnimationTime;  // Stores the last time the animation was updated

  uint32_t interpolateColors(uint32_t colorFrom, uint32_t colorTo, uint8_t position);
  uint32_t startingColor;
  uint32_t endingColor;

public:
  StatusLED(uint16_t num_pixels, uint8_t pin);
  void begin();  // Setup the LED strip

  // Update the animation state and display it on the LEDs
  void update();

  void blink();

  void setUpdateIntervalMillis(uint16_t interval);

  void setStartingColor(uint8_t r, uint8_t g, uint8_t b);
  void setEndingColor(uint8_t r, uint8_t g, uint8_t b);

  void setStartingColorHSV(float h, float s, float v);
  void setEndingColorHSV(float h, float s, float v);
  void setStartingColorHSV(ColorConverter::HSV hsv);
  void setEndingColorHSV(ColorConverter::HSV hsv);
};

#endif
