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
  //setColors(currentState);

  Serial.begin(9600);
}

void loop() {
  statusLed.update();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // Guardamos la última vez que cambiamos los colores
    previousMillis = currentMillis;

    // // Efecto llamativo para cuando cambiamos de estado
    // for (uint16_t i = 0; i < 10; i++) {
    //   statusLed.blink();
    // }

    currentState++;
    if (currentState > numStates) {
      currentState = 0;
    }

    //setColors(currentState);
  }

  ColorConverter::HSV startingColor;
  ColorConverter::HSV endingColor;

  startingColor = ColorConverter::HSVColor(0.0, 1.0, 0.6);
  endingColor = ColorConverter::HSVColor(0.0, 1.0, 0.6);

  statusLed.setStartingColorHSV(startingColor);
  statusLed.setEndingColorHSV(endingColor);
}

//void setColors(int state) {
//   ColorConverter::HSV startingColor;
//   ColorConverter::HSV endingColor;

//   switch (state) {
//     case 0:
//       startingColor = ColorConverter::HSVColor(0.0, 1.0, 0.6);
//       endingColor = ColorConverter::HSVColor(120.0, 0.8, 0.4);
//       Serial.println("0");
//       statusLed.setUpdateIntervalMillis(5);

//       break;
//     case 1:
//       startingColor = ColorConverter::HSVColor(60.0, 1.0, 1.0);
//       endingColor = ColorConverter::HSVColor(180.0, 0.8, 0.6);
//       Serial.println("1");
//       statusLed.setUpdateIntervalMillis(10);

//       break;
//     case 2:
//       startingColor = ColorConverter::HSVColor(120.0, 1.0, 0.8);
//       endingColor = ColorConverter::HSVColor(240.0, 0.6, 0.4);
//       Serial.println("2");
//       statusLed.setUpdateIntervalMillis(15);


//     case 3:
//       startingColor = ColorConverter::HSVColor(0.0, 1.0, 0.8);
//       endingColor = ColorConverter::HSVColor(30.0, 1.0, 0.8);
//       Serial.println("3");
//       statusLed.setUpdateIntervalMillis(20);

//       break;

//     default:
//       //setRandomState();
//       return;
//   }

//   statusLed.setStartingColorHSV(startingColor);
//   statusLed.setEndingColorHSV(endingColor);
// }
