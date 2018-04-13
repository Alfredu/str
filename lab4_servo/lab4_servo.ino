/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(9, 800,2200);  // attaches the servo on pin 9 to the servo object
  myservo.write(90);
  delay(1000);
}

void loop() {

  //maximum speed
  // 0.16 s - 60 deg => 480 ms - 180 deg (and we make it 500 just to make sure that it achieves its final position)
  myservo.write(180);
  delay(500);
  myservo.write(0);
  delay(500);

  //avarage speed (2x slower than maximum)
  // 0.16 s - 60 deg /:60
  // 0.0027 - 1 deg
  // to make it 2x slower we need to wait 2x0.027s=0.054s
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(5.4);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(5.4);                       // waits 15ms for the servo to reach the position
  }

    //avarage speed (5x slower than maximum)
  // 0.16 s - 60 deg /:60
  // 0.0027 - 1 deg
  // to make it 5x slower we need to wait 5x0.027s=0.135s
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(13.5);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(13.5);                       // waits 15ms for the servo to reach the position
  }
}

