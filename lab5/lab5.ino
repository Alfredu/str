
#include <Servo.h>
#include "PIDball.h"
#include "hcsr04.h"


Servo myservo;  // create servo object to control a servo
HCSR04 distSensor;
PIDball controller;

void setup() {
  distSensor.initiate();
  myservo.attach(9, 800,2350);  // attaches the servo on pin 9 to the servo object
  myservo.write(73);
  delay(1000);
  Serial.begin(9600);
  controller.setGoal(17.0);
  controller.setTunings(6,0,0.001);
}

void loop() {
  double t0=millis();
  // current distance
  double input = distSensor.getDistance();
  // current position of the servo
  
  double currentMotPos = myservo.read();
  // goal position of the servo
  double output = controller.compute(input, currentMotPos);
  myservo.write(output);
  double t1=millis();
  Serial.println(t1-t0);
}
