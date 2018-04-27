
#include <Servo.h>
#include "PIDball.h"
#include "HCSR04.h"


Servo myservo;  // create servo object to control a servo
HCSR04 distSensor;
PIDball controller;

void setup() {
  distSensor.initiate();
  myservo.attach(9, 800,2350);  // attaches the servo on pin 9 to the servo object
  myservo.write(0);
  delay(1000);
  Serial.begin(9600);
  controller.setGoal(12.0);
  controller.setTunings(0.1,0,0);
}

void loop() {
  // put your main code here, to run repeatedly:
  double Input = distSensor.getDistance();
  double Output = controller.Compute(Input);
  myservo.write();
  
}
