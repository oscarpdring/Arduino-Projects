#include "ColorConverter.h"

ColorConverter::HSV ColorConverter::HSVColor(float h, float s, float v) {
    HSV color;
    color.h = h;
    color.s = s;
    color.v = v;
    return color;
}

ColorConverter::RGB ColorConverter::hsvToRgb(ColorConverter::HSV hsv) {
  return ColorConverter::hsvToRgb(hsv.h, hsv.s, hsv.v);
}

ColorConverter::RGB ColorConverter::hsvToRgb(float h, float s, float v) {
  int i;
  float f, p, q, t;
  RGB rgb;


  if (s == 0.0) {
    // Grayscale, just set the three RGB components to the brightness.
    rgb.r = rgb.g = rgb.b = v * 255;
    return rgb;
  }

  h /= 60.0;   // Split the hue into 6 sectors (each 60 degrees).
  i = int(h);  // Get the sector index as an integer.
  f = h - i;   // Calculate fractional part of the hue.

  p = v * (1.0 - s);
  q = v * (1.0 - s * f);
  t = v * (1.0 - s * (1.0 - f));

  switch (i) {
    case 0:
      rgb.r = v * 255;
      rgb.g = t * 255;
      rgb.b = p * 255;
      break;
    case 1:
      rgb.r = q * 255;
      rgb.g = v * 255;
      rgb.b = p * 255;
      break;
    case 2:
      rgb.r = p * 255;
      rgb.g = v * 255;
      rgb.b = t * 255;
      break;
    case 3:
      rgb.r = p * 255;
      rgb.g = q * 255;
      rgb.b = v * 255;
      break;
    case 4:
      rgb.r = t * 255;
      rgb.g = p * 255;
      rgb.b = v * 255;
      break;
    default:  // case 5:
      rgb.r = v * 255;
      rgb.g = p * 255;
      rgb.b = q * 255;
      break;
  }

  return rgb;
}