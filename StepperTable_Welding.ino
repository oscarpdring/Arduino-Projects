
#include "Arduino.h"
#include <AccelStepper.h>

#define PIN_ENABLE 2
#define PIN_PEDAL_DER 6
#define PIN_PEDAL_IZQ 5
#define PIN_POTENCIOMETRO A0


AccelStepper stepperTable(1, 4, 3); //PULL PIN 4;  DIR PIN 3

int maxSpeedVar = 3000;
int maxAccelVar = 10000;

int speedVar = 600;         //Speed variable parameter
int accelVar = 8000;        //Acceleration variable parameter

float stepsPerRev = 12800;  //Steps per Revolution set on the driver
int gearRelation = 3;       //Gear relation parameter

int velRelation = 2;
int velocidad = 500;
int posPedalDer = 1;
int posPedalIzq = 1;
int iterationLoop = 0;
int iterationLoopPar = 10000;

char revolutions;
float stepsToMove;

void setup() {
  //Serial.begin(9600);

  // put your setup code here, to run once:
  stepperTable.setMaxSpeed(maxSpeedVar);
  stepperTable.setAcceleration(maxAccelVar);
  stepperTable.setAcceleration(accelVar);
  stepperTable.setSpeed(speedVar);
  stepperTable.setEnablePin(PIN_ENABLE); //ENABLE PIN 2
  stepperTable.enableOutputs(); //Free the motor

  pinMode(PIN_PEDAL_DER, INPUT_PULLUP);
  pinMode(PIN_PEDAL_IZQ, INPUT_PULLUP);
  pinMode(PIN_POTENCIOMETRO, INPUT);

}


void medirPotenciometro() {
  int medida;

  medida = analogRead(PIN_POTENCIOMETRO);
  velocidad = 100 + velRelation * medida;
}

void medirPedales() {
  posPedalDer = digitalRead(PIN_PEDAL_DER);
  posPedalIzq = digitalRead(PIN_PEDAL_IZQ);

}
void loop() {
  stepperTable.runSpeed();
  iterationLoop = iterationLoop + 1;

  if (iterationLoop >= iterationLoopPar) {
    medirPedales();
    if (posPedalDer == 0) { //Se pulsa el pedal derecho
      medirPotenciometro();
      
      stepperTable.setSpeed(velocidad);
    }

    if (posPedalIzq == 0) { //Se pulsa el pedal izquierdo
      medirPotenciometro();
      velocidad = velocidad * -1;
      stepperTable.setSpeed(velocidad);
    }

    if (posPedalDer == 1 && posPedalIzq == 1) {
      stepperTable.setSpeed(0);
    }
    
    iterationLoop = 0;
  }
}
