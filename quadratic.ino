#include <Arduino.h>

// Define your quadratic regression equation coefficients
float a = 1.25599;
float b = -150.88636;
float c = 5373.724;

void setup() {
  Serial.begin(115200);
  delay(1000); // Delay to allow serial monitor to open
  
  // Test the equation with a value of x
  int x_value = 50;
  float y = solveQuadraticEquation(x_value);
  
  Serial.print("For x = ");
  Serial.print(x_value);
  Serial.print(", y = ");
  Serial.println(y);
}

void loop() {
  // Nothing to do in the loop
}

float solveQuadraticEquation(int x) {
  return a * pow(x, 2) + b * x + c;
}