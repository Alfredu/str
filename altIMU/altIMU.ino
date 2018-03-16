#include <avr/io.h>
#include <avr/interrupt.h>
#include "altIMU.h"
/*
 * This program displays the distance in cm measured with 
 * the sensor HC-SR04.
 *
 * To do so, we use the 16 bit Timer1 and the Logic-level change interruption 4.
 * We decided to use Timer1 to maximize timing and distance measuring accuracy.
 * We decided to use interruption 4 to avoid clashing with other functions such as i2c.
 */
 


altIMU sensor;
void setup() {
  Serial.begin(9600);
  sensor = altIMU(1);
}


void loop() {

  /*repeatedly creating triggers for the sensor and printing
    the distance value*/
  int val = sensor.readValue();
  
  Serial.println(val);
}



