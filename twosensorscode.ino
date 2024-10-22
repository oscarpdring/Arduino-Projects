/*
  * Original Library soruce: https://github.com/adafruit/Adafruit_VL53L0X
  * 
 * Using 2 VL53L0X Laser Distance Sensors
 * 
 * 
 * Watch video instructions for this code: https://youtu.be/0glBk917HPg
 * 
Updated by Ahmad Shamshiri in April 2020
 
 * in Ajax, Ontario, Canada. www.robojax.com
 * 
  Need wiring diagram from this code:  https://youtu.be/0glBk917HPg
  Purchase My Arduino course on Udemy.com http://robojax.com/L/?id=62
 * 

 * Get this code and other Arduino codes from Robojax.com
Learn Arduino step by step in structured course with all material, wiring diagram and library
all in once place. Purchase My course on Udemy.com http://robojax.com/L/?id=62

If you found this tutorial helpful, please support me so I can continue creating 
content like this. You can support me on Patreon http://robojax.com/L/?id=63

or make donation using PayPal http://robojax.com/L/?id=64

 *  * This code is "AS IS" without warranty or liability. Free to be used as long as you keep this note intact.* 
 * This code has been download from Robojax.com
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "Adafruit_VL53L0X.h"

// address we will assign if dual sensor is present
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31
int sensor1, sensor2;


// set the pins to shutdown
#define SHT_LOX1 7
#define SHT_LOX2 6

// objects for the vl53l0x
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();

// this holds the measurement
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;

// Data arrays for each sensor
const int numReadings = 5;
int sensor1Data[numReadings];
int sensor2Data[numReadings];

int num_points = 12;


int values[] = { 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40 };                                                                                       // Initialize array with desired values
int resolution[] = { 947, 965, 996, 1024, 1062, 1097, 1125, 1156, 1212, 1247, 1306, 1346 };                                                              // resultion corresponding to the distance in cm for the sensor
float pixel[] = { 0.0026822, 0.0026321, 0.0025502, 0.0024805, 0.0023917, 0.0023154, 0.0022578, 0.0021972, 0.0020957, 0.0020369, 0.0019449, 0.0018871 };  // pixel size in cm

// Index to keep track of the current position in the data arrays
int currentIndex = 0;


/*
    Reset all sensors by setting all of their XSHUT pins low for delay(10), then set all XSHUT high to bring out of reset
    Keep sensor #1 awake by keeping XSHUT pin high
    Put all other sensors into shutdown by pulling XSHUT pins low
    Initialize sensor #1 with lox.begin(new_i2c_address) Pick any number but 0x29 and it must be under 0x7F. Going with 0x30 to 0x3F is probably OK.
    Keep sensor #1 awake, and now bring sensor #2 out of reset by setting its XSHUT pin high.
    Initialize sensor #2 with lox.begin(new_i2c_address) Pick any number but 0x29 and whatever you set the first sensor to
 */


void setID() {
  // all reset
  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  delay(10);
  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  // activating LOX1 and reseting LOX2
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);

  // initing LOX1
  if (!lox1.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    while (1)
      ;
  }
  delay(10);

  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  //initing LOX2
  if (!lox2.begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while (1)
      ;
  }
}

void setup() {
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (!Serial) { delay(1); }

  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);

  Serial.println("Shutdown pins inited...");

  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);

  Serial.println("Both in reset mode...(pins are low)");


  Serial.println("Starting...");
  setID();
}



void read_dual_sensors() {

  lox1.rangingTest(&measure1, false);  // pass in 'true' to get debug data printout!
  lox2.rangingTest(&measure2, false);  // pass in 'true' to get debug data printout!

  // print sensor one reading
  Serial.print("1: ");
  if (measure1.RangeStatus != 4) {  // if not out of range
    sensor1 = measure1.RangeMilliMeter;
    sensor1Data[currentIndex] = sensor1;
    Serial.print(sensor1);
    Serial.print("mm");
  } else {
    Serial.print("Out of range");
  }

  Serial.print(" ");

  // print sensor two reading
  Serial.print("2: ");
  if (measure2.RangeStatus != 4) {
    sensor2 = measure2.RangeMilliMeter;
    sensor2Data[currentIndex] = sensor2;
    Serial.print(sensor2);
    Serial.print("mm");
  } else {
    Serial.print("Out of range");
  }

  Serial.println();


  // Increment index, and wrap around if necessary
  currentIndex++;
  if (currentIndex >= numReadings) {
    currentIndex = 0;
  }
}

// Function to calculate the average of corresponding elements in two arrays
int calculateAverage() {
  int sum = 0;
  int validReadings = 0;  // Counter for valid readings
  int average = 0;        // Declare average variable here

  // Iterate through the arrays and sum up the corresponding elements
  for (int i = 0; i < numReadings; i++) {
    // Check if both readings are valid (not equal to -1)
    if (sensor1Data[i] != -1 && sensor2Data[i] != -1) {
      sum += (sensor1Data[i] + sensor2Data[i]);  // Add the sum of both readings
      validReadings++;                           // Increment the counter for valid readings
    }
  }

  // // Calculate the average
  if (validReadings > 0) {
    average = sum / (2 * validReadings);  // Divide the sum by twice the number of valid readings
    Serial.print("Average of Sensor 1 and Sensor 2: ");
    Serial.print(average);
    Serial.println("mm");
    // } else {
    //   Serial.println("No valid readings for averaging.");
  }


  // use the average to solve the linear equation


  int sensorValue = average;  // Example sensor value (you can replace it with actual sensor value)
  // Define your linear regression equation coefficients
  float m = -36.59091;  // Slope
  float c = 2788.4697;  // Intercept
  int X_value = average;
  float Y = m * X_value + c;;
  Serial.print("For X = ");
  Serial.print(X_value);
  Serial.print(", Y = ");
  Serial.println(Y);



  // // Find the position of the sensor value in the values array
  // int position = -1;
  // for (int i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
  //   if (values[i] == sensorValue) {
  //     position = i;
  //     break;  // Exit loop once the value is found
  //   }
  // }

  // // Check if the sensor value was found in the values array
  // if (position != -1) {
  //   // Print corresponding resolution and pixel number
  //   Serial.print("Resolution: ");
  //   Serial.println(resolution[position]);
  //   Serial.print("Pixel: ");
  //   Serial.println(pixel[position], 7);
  // } else {
  //   Serial.println("Sensor value not found in the values array");
  // }

  // return average;
}





void loop() {
  // Calculate average of both sensors
  //linearRegressionResolution();

  calculateAverage();
  read_dual_sensors();
  delay(2000);
}