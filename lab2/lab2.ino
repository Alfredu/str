#include "altIMU.h"
#include "hcsr04.h"

/* 
 *  Laboratory 2: STR
 *  
 * This program displays the values measured with two sensors
 * at the same time: the value of the angle around Y-axis of accelerometer
 * and the distance measured with HCSR04 sensor.
 */

altIMU accelerometer;
HCSR04 distSensor;

void setup() {
 
  Serial.begin(115200);
  accelerometer.initiate();
  distSensor.initiate();
}

void loop() {

  float angle = accelerometer.getAngleAroundYAxis();
  float dist = distSensor.getDistance();
  
  Serial.print("Angle around Y-axis: ");
  Serial.print(angle);
  Serial.print("\tDistance: ");
  Serial.print(dist);
  Serial.print("\n");
  
}
