#ifndef COLORCONVERTER_H
#define COLORCONVERTER_H

#include <stdint.h>

class ColorConverter {
public:

  struct RGB
  {
    uint8_t r;  // 0-255
    uint8_t g;  // 0-255
    uint8_t b;  // 0-255
  };

  struct HSV
  {
    float h;  // 0.0-359.0
    float s;  // 0.0 - 1.0
    float v;  // 0.0 - 1.0
  };

  // Color creation
  static HSV HSVColor(float h, float s, float v);

  // Static method tos convert from HSV to RGB
  static RGB hsvToRgb(float h, float s, float v);
  static RGB hsvToRgb(HSV hsv);
};

#endif
