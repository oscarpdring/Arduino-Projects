#include "Arduino.h"
#include <Stepper.h>

#define PIN_ENABLE 8
#define PIN_PEDAL_RIGHT 6
#define PIN_PEDAL_LEFT 7
#define PIN_POTENTIOMETER A1
#define PIN_SHUTTING 10

const int stepsPerRevolution = 12800; // Steps per Revolution set on the driver
const int stepsPerDegree = stepsPerRevolution / 360; // Steps per degree

Stepper stepperTable(stepsPerRevolution, 3, 4, 5, 6); // Assuming 4-wire stepper, adjust pins accordingly

int speedVar = 1000; // Speed variable parameter

int setDeg = 10; // Desired degrees to move

float numberStops;
int stops = 0;
bool shutCameraLoop = false;
int shuttingDelay = 750;

int velRelation = 2;
int speed = 500;
int posPedalRight = 1;
int posPedalLeft = 1;
int iterationLoop = 0;
int iterationLoopPar = 10000;

int stepsToMove;

void setup() {
  Serial.begin(9600);

  // Setup code to run once
  pinMode(PIN_ENABLE, OUTPUT);
  pinMode(PIN_PEDAL_RIGHT, INPUT_PULLUP);
  pinMode(PIN_PEDAL_LEFT, INPUT_PULLUP);
  pinMode(PIN_POTENTIOMETER, INPUT);
  pinMode(PIN_SHUTTING, OUTPUT);
  digitalWrite(PIN_SHUTTING, LOW);

  // Initialize stepper speed
  stepperTable.setSpeed(speedVar);
  digitalWrite(PIN_ENABLE, HIGH); // Enable the motor (adjust according to your driver)
}

void calculateDegree() {
  stepsToMove = setDeg * stepsPerDegree;
  numberStops = 360 / setDeg;
  Serial.println(numberStops);
}

void readPotentiometer() {
  int measurement = analogRead(PIN_POTENTIOMETER);
  speed = 100 + velRelation * measurement;
  stepperTable.setSpeed(speed);
}

void readPedals() {
  posPedalRight = digitalRead(PIN_PEDAL_RIGHT);
  posPedalLeft = digitalRead(PIN_PEDAL_LEFT);
}

void loop() {
  iterationLoop = iterationLoop + 1;

  if (iterationLoop >= iterationLoopPar) {
    // USING PEDALS
    readPedals();
    if (posPedalRight == 0) { // Right pedal is pressed
      calculateDegree();
      Serial.println(stepsToMove);
      stops = 0;
      shutCameraLoop = true;
    }

    if (posPedalLeft == 0) { // Left pedal is pressed
      shutCameraLoop = false;
      stepsToMove = -stepperTable.currentPosition(); // Move back to the start
      stepperTable.step(stepsToMove);
    }

    // Shutting loop
    if (shutCameraLoop == true && stops < numberStops) {
      if (stops == 0 || stepperTable.currentPosition() % stepsToMove == 0) {
        delay(shuttingDelay);
        // Shut Camera
        digitalWrite(PIN_SHUTTING, HIGH);
        delay(300);
        digitalWrite(PIN_SHUTTING, LOW);
        delay(300);
        // MOVE stepsToMove steps
        stepperTable.step(stepsToMove);
        stops = stops + 1;
      }
    }

    iterationLoop = 0;
  }
}
