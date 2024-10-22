#include "StatusLED.h"
#include "ColorConverter.h"
#include "MotionControl.h"

#define LED_PIN 11
#define LED_COUNT 2

StatusLED statusLed(LED_COUNT, LED_PIN);

enum SystemState {
  Idle,
  Scanning,
  Complete,
  Error
};

SystemState currentSystemState = Idle;
unsigned long previousMillis = 0;
const long interval = 10000;  // 10 seconds interval

// Timer for the Complete state
unsigned long completeTimerStart = 0;
const long completeDuration = 30000;  // 30 seconds

void setup() {
  statusLed.begin();
  setColors(currentSystemState);
  Serial.begin(9600);
}

void loop() {
  statusLed.update();

  unsigned long currentMillis = millis();

  if (currentSystemState == Complete && (currentMillis - completeTimerStart >= completeDuration)) {
    // Timer expired, return to Idle state after displaying blue for 30 seconds
    currentSystemState = Idle;
    setColors(currentSystemState);
  }

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    for (uint16_t i = 0; i < 10; i++) {
      statusLed.blink();
    }

    // Update system state
    updateSystemState();

    // Set colors based on the current system state
    setColors(currentSystemState);
  }
}

void updateSystemState() {
  // Add your logic to update the system state based on the received commands and parameters.
  // For now, let's simulate a change between Idle, Scanning, Complete, and Error states.
  if (currentSystemState == Idle) {
    currentSystemState = Scanning;
  } else if (currentSystemState == Scanning) {
    // Simulate an error randomly during scanning (adjust as needed)
    if (random(100) < 5) {
      currentSystemState = Error;
    } else {
      currentSystemState = Complete;
      completeTimerStart = millis();  // Start the timer for the Complete state
    }
  } else {
    currentSystemState = Idle;  // Reset to Idle after Complete or Error
  }
}

void setColors(SystemState state) {
  ColorConverter::HSV startingColor;
  ColorConverter::HSV endingColor;

  switch (state) {
    case Idle:
      startingColor = ColorConverter::HSVColor(120.0, 1.0, 0.0);  // Green
      endingColor = ColorConverter::HSVColor(120.0, 1.0, 0.6);  // Green
      statusLed.setUpdateIntervalMillis(5);
      break;
    case Scanning:
      startingColor = ColorConverter::HSVColor(30.0, 1.0, 1.0);  // Orange
      endingColor = ColorConverter::HSVColor(30.0, 1.0, 0.0);  // Orange
      statusLed.setUpdateIntervalMillis(10);
      break;
    case Complete:
      startingColor = ColorConverter::HSVColor(210.0, 1.0, 1.0);  // Blue
      endingColor = ColorConverter::HSVColor(210.0, 1.0, 1.0);  // Blue
      statusLed.setUpdateIntervalMillis(5);
      break;
    case Error:
      startingColor = ColorConverter::HSVColor(0.0, 1.0, 1.0);  // Red
      endingColor = ColorConverter::HSVColor(0.0, 1.0, 0.0);  // Red
      statusLed.setUpdateIntervalMillis(5);
      break;
  }

  statusLed.setStartingColorHSV(startingColor);
  statusLed.setEndingColorHSV(endingColor);
}
