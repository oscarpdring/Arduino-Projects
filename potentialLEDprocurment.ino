#include <Adafruit_NeoPixel.h>
#include "MotionControl.h"

#define LED_PIN    A2  // Replace with the actual pin number you are using
#define LED_COUNT  2   // Replace with the number of LEDs in your strip

Adafruit_NeoPixel ledStrip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

enum SystemState {
  Idle,
  Active
};

SystemState currentSystemState = Idle;

void setup() {
  ledStrip.begin();
  ledStrip.show();  // Initialize all pixels to 'off'
}

void loop() {
  // Check the system state from your other script
  checkSystemState();

  // Set LED colors based on the system state
  setColors(currentSystemState);

  // Add any additional logic or delay as needed
  delay(1000);
}

void checkSystemState() {
  // Check the state variable from the other script (replace with the actual variable)
  // For demonstration purposes, assume that motionControl.state indicates the system state
  // You may need to adjust this based on the actual variable in your script
  if (motionControl.state == IDLE) {
    currentSystemState = Idle;
  } else {
    currentSystemState = Active;
  }
}

void setColors(SystemState state) {
  ColorConverter::HSV startingColor;
  ColorConverter::HSV endingColor;

  switch (state) {
    case Idle:
      startingColor = ColorConverter::HSVColor(120.0, 1.0, 0.0);  // Green
      endingColor = ColorConverter::HSVColor(120.0, 1.0, 0.6);  // Green
      ledStrip.setUpdateIntervalMillis(5);
      break;
    case Active:
      startingColor = ColorConverter::HSVColor(255.0, 1.0, 0.0);  // Red
      endingColor = ColorConverter::HSVColor(255.0, 1.0, 0.6);  // Red
      ledStrip.setUpdateIntervalMillis(10);
      break;
  }

  ledStrip.setStartingColorHSV(startingColor);
  ledStrip.setEndingColorHSV(endingColor);
  ledStrip.show();
}
