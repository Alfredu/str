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
  myservo.attach(9, 800,2350);  // attaches the servo on pin 9 to the servo object
  myservo.write(0);
  delay(1000);
}
  //avarage speed ((speed_divider) times slower than maximum)
  // 0.16 s - 60 deg /:60 (375 deg/s)
  // 0.0027 - 1 deg
  // to make it (speed_divider) x slower we need to wait (speed_divider)x0.027s
  //we will use the integer 3 to make sure we wait long enough.
void servo_revolution(int speed_divider)
{
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(3 * speed_divider);                       
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(3 * speed_divider);                       
  }    
}
void loop() {

  //maximum speed
  // 0.16 s - 60 deg => This will run at 375 degrees/second
  servo_revolution(1);
  delay(100);
  //average speed
  //Since we are diving the speed by 2, this will run at (375/2) = 187.5 degrees/second
  servo_revolution(2);
  delay(100);
  //low speed
  //Since we are diving the speed by 5, this will run at (375/5) = 75 degrees/second
  servo_revolution(5);
  delay(1000);
}

