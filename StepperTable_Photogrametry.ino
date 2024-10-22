
#include "Arduino.h"
#include <AccelStepper.h>

#define PIN_ENABLE 2 //ENA previously 8
#define PIN_PEDAL_DER 6
#define PIN_PEDAL_IZQ 5 // previously 7 
#define PIN_POTENCIOMETRO A0// previoudsly A1
#define PIN_SHUTTING A5 //previously 10


AccelStepper stepperTable(1, 4, 3); //PULL PIN 4; DIR PIN 3//// previously 3, 4

int maxSpeedVar = 3000;
int maxAccelVar = 10000;

int speedVar = 1000;         //Speed variable parameter
int accelVar = 8000;        //Acceleration variable parameter

float stepsPerRev = 12800;  //Steps per Revolution set on the driver
int gearRelation = 3;       //Gear relation parameter
//float degPerRev = (1/360);
float degPerRev = .00277777777;


int setDeg = 5;        //SPACE DEGREES WANTED

float numberStops;
int stops = 0;
float stepDeg;
bool shutCameraLoop = false;
int shuttingDelay = 750;


int velRelation = 2;
int velocidad = 400;
int posPedalDer = 1;
int posPedalIzq = 1;
int iterationLoop = 0;
int iterationLoopPar = 10000;

char revolutions;
float stepsToMove;


void setup() {
  Serial.begin(9600);

  // put your setup code here, to run once:
  stepperTable.setMaxSpeed(maxSpeedVar);
  stepperTable.setAcceleration(maxAccelVar);
  stepperTable.setAcceleration(accelVar);
  stepperTable.setSpeed(speedVar);
  stepperTable.setEnablePin(PIN_ENABLE); //ENABLE PIN 2
  stepperTable.enableOutputs(); //Free the motor
  stepperTable.setCurrentPosition(0);

  pinMode(PIN_PEDAL_DER, INPUT_PULLUP);
  pinMode(PIN_PEDAL_IZQ, INPUT_PULLUP);
  pinMode(PIN_POTENCIOMETRO, INPUT);
  pinMode(PIN_SHUTTING, OUTPUT);
  digitalWrite(PIN_SHUTTING,LOW);

}

void calcularDegree(){

  stepDeg = setDeg * stepsPerRev * gearRelation * degPerRev;
  numberStops = 360/setDeg;
  Serial.println(numberStops);
  
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
  stepperTable.run();
  iterationLoop = iterationLoop + 1;

  if (iterationLoop >= iterationLoopPar) {
    // USING PEDALS
    medirPedales();
    if (posPedalDer == 0) { //Se pulsa el pedal derecho
      calcularDegree();
      Serial.println(stepDeg);
      stops = 0;
      shutCameraLoop = true;
      
    }

    if (posPedalIzq == 0) { //Se pulsa el pedal izquierdo
      shutCameraLoop = false;
      stepperTable.move(velocidad);
    }

    /// shutting loop
    if(shutCameraLoop == true && stops < numberStops){
      
      if(stepperTable.distanceToGo() == 0){
        delay(shuttingDelay);
        //Shut Camara
        digitalWrite(PIN_SHUTTING,HIGH);
        delay(300);
        digitalWrite(PIN_SHUTTING,LOW);
        delay(300);
        //MOVE stepDeg steps
        stepperTable.move(stepDeg);
        stops = stops + 1;
      }
    }
    
    iterationLoop = 0;
  }
}
