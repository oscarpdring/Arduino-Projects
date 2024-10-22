#include <Arduino.h>



//using data provided by Inez's hand measured and calculated resolution testing, this linear regression model was create. 

// Define your linear regression equation coefficients
float m1 = -36.59091; // Slope
float c1 = 2788.4697; // Intercept

float m2 =  0.00007;
float c2 = - 0.00018; 
char buffer[20];

void setup() {
  Serial.begin(115200);
  delay(1000); // Delay to allow serial monitor to open
  
  // Test the equation with a value of X
  int X_value = 50;
  float Y1 = solveLinearEquation1(X_value);
  float Y2 = solveLinearEquation2(X_value);
  
  Serial.print("For Distance = ");
  Serial.print(X_value);
  Serial.print(" cm");
  Serial.print(", Resolution = ");
  Serial.println(Y1);
  Serial.print("For Distance = ");
  Serial.print(X_value);
  Serial.print(" cm");
  Serial.print(", Pixel Size = ");
  dtostrf(Y2,1,10,buffer);
  Serial.println(buffer);


}

void loop() {
  //take input from the sensor, and convert use the sensors value as the X intput
  //once the Y(resolution) value is obtained
}

float solveLinearEquation1(int X) {
  return m1 * X + c1;
  
}

float  solveLinearEquation2(int X) {
return m2 * X + c2;
}
