#include "StatusLED.h"


StatusLED::StatusLED(uint16_t num_pixels, uint8_t pin)
  : ledStrip(num_pixels, pin, NEO_GRB + NEO_KHZ800) {
  progress = 0;
  totalPixels = num_pixels;
  updateIntervalMillis = 10;
  lastAnimationTime = 0;
}

void StatusLED::begin() {
  ledStrip.begin();
  ledStrip.show();
  ledStrip.setBrightness(50);
}

void StatusLED::setStartingColor(uint8_t r, uint8_t g, uint8_t b) {
  startingColor = ledStrip.Color(r, g, b);
}

void StatusLED::setEndingColor(uint8_t r, uint8_t g, uint8_t b) {
  endingColor = ledStrip.Color(r, g, b);
}

void StatusLED::setStartingColorHSV(float h, float s, float v) {
  ColorConverter::RGB rgb = ColorConverter::hsvToRgb(h, s, v);
  setStartingColor(rgb.r, rgb.g, rgb.b);
}

void StatusLED::setEndingColorHSV(float h, float s, float v) {
  ColorConverter::RGB rgb = ColorConverter::hsvToRgb(h, s, v);
  setEndingColor(rgb.r, rgb.g, rgb.b);
}

void StatusLED::setStartingColorHSV(ColorConverter::HSV hsv) {
  ColorConverter::RGB rgb = ColorConverter::hsvToRgb(hsv);
  setStartingColor(rgb.r, rgb.g, rgb.b);
}

void StatusLED::setEndingColorHSV(ColorConverter::HSV hsv) {
  ColorConverter::RGB rgb = ColorConverter::hsvToRgb(hsv);
  setEndingColor(rgb.r, rgb.g, rgb.b);
}

void StatusLED::setUpdateIntervalMillis(uint16_t interval)
{
  updateIntervalMillis = interval;
}

void StatusLED::update() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastAnimationTime >= updateIntervalMillis) {
    lastAnimationTime = currentMillis;

    for (uint16_t i = 0; i < totalPixels; i++) {
      ledStrip.setPixelColor(i, interpolateColors(startingColor, endingColor, (i + progress) & 255));
    }
    ledStrip.show();
    
    progress++;
    if (progress >= 255) {
      progress = 0;
      uint32_t temporaryColor = startingColor;
      startingColor = endingColor;
      endingColor = temporaryColor;
    }
  }
}

void StatusLED::blink() {

  // Apaga todos los LEDs
  for (uint16_t i = 0; i < totalPixels; i++) {
    ledStrip.setPixelColor(i, ledStrip.Color(0, 0, 0));
  }
  ledStrip.show();
  delay(100);  // Espera 100ms
  
  // Enciende todos los LEDs con el color de inicio
  for (uint16_t i = 0; i < totalPixels; i++) {
    ledStrip.setPixelColor(i, startingColor);
  }
  ledStrip.show();
  delay(100);  // Espera 100ms

}

uint32_t StatusLED::interpolateColors(uint32_t colorFrom, uint32_t colorTo, uint8_t position) {
  byte r_from = (colorFrom >> 16) & 0xFF;
  byte g_from = (colorFrom >> 8) & 0xFF;
  byte b_from = colorFrom & 0xFF;

  byte r_to = (colorTo >> 16) & 0xFF;
  byte g_to = (colorTo >> 8) & 0xFF;
  byte b_to = colorTo & 0xFF;

  byte r_result = r_from + (position * (r_to - r_from) / 255);
  byte g_result = g_from + (position * (g_to - g_from) / 255);
  byte b_result = b_from + (position * (b_to - b_from) / 255);

  return ledStrip.Color(r_result, g_result, b_result);
}
