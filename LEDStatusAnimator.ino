#include "StatusLED.h"
#include "ColorConverter.h"

#define LED_PIN 11
#define LED_COUNT 2

StatusLED statusLed(LED_COUNT, LED_PIN);

unsigned long previousMillis = 0;
const long interval = 10000;  // intervalo de 10 segundos

int currentState = 0;
int numStates = 3;


void setup() {
  statusLed.begin();
  setColors(currentState);
  
  Serial.begin(9600);
}

void loop() {
  statusLed.update();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // Guardamos la última vez que cambiamos los colores
    previousMillis = currentMillis;

    // Efecto llamativo para cuando cambiamos de estado
    for (uint16_t i = 0; i < 10; i++) {
      statusLed.blink();
    }

    currentState++;
    if (currentState > numStates) {
      currentState = 0;
    }

    setColors(currentState);
  }
}

void setColors(int state) {
  ColorConverter::HSV startingColor;
  ColorConverter::HSV endingColor;

  switch (state) {
    case 0:
      startingColor = ColorConverter::HSVColor(0.0, 1.0, 0.6);
      endingColor = ColorConverter::HSVColor(120.0, 0.8, 0.4);
      statusLed.setUpdateIntervalMillis(5);
      break;
    case 1:
      startingColor = ColorConverter::HSVColor(60.0, 1.0, 1.0);
      endingColor = ColorConverter::HSVColor(180.0, 0.8, 0.6);
      statusLed.setUpdateIntervalMillis(10);
      break;
    case 2:
      startingColor = ColorConverter::HSVColor(120.0, 1.0, 0.8);
      endingColor = ColorConverter::HSVColor(240.0, 0.6, 0.4);
      statusLed.setUpdateIntervalMillis(15);

    case 3:
      startingColor = ColorConverter::HSVColor(0.0, 1.0, 0.8);
      endingColor = ColorConverter::HSVColor(30.0, 1.0, 0.8);
      statusLed.setUpdateIntervalMillis(20);
      break;

    default:
      //setRandomState();
      return;
  }

  statusLed.setStartingColorHSV(startingColor);
  statusLed.setEndingColorHSV(endingColor);
}

/*
float randomFloat(float a, float b) {
  int randNumber = random((int)(a * 1000), (int)(b * 1000));
  float fl = (float)randNumber / 1000.00;
  return fl;
}

void setRandomState() {
  ColorConverter::HSV startingColor = ColorConverter::HSVColor(random(359), randomFloat(0.1, 0.9), 0.8);
  ColorConverter::HSV endingColor = ColorConverter::HSVColor(random(359), 0.6, 0.4);

  statusLed.setStartingColorHSV(startingColor);
  statusLed.setEndingColorHSV(endingColor);
  uint16_t t = random(5, 35);
  statusLed.setUpdateIntervalMillis(t);

  String s = "StartingColor - H:";
  s += String(startingColor.h) + " S:" + String(startingColor.s) + " V:" + String(startingColor.v);
  Serial.println(s);

  s = "EndingColor - H:";
  s += String(endingColor.h) + " S:" + String(endingColor.s) + " V:" + String(endingColor.v);
  Serial.println(s);

  s = "UpdateIntervalMillis: ";
  s += t;
  Serial.println(s);
}

LEDStatusAnimator
NeoPixelColorTransition
DynamicLEDColorizer
ArduinoLEDMoodChanger
WS2811ColorFlow
LEDStatusBlender
ArduinoColorMorph
NeoPixelStateShifter
LEDStatefulAnimator
ArduinoLEDWaveform

*/
