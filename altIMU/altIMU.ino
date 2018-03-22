#include <avr/io.h>
#include <avr/interrupt.h>
#include "altIMU.h"
/*
 * This programme measure the Euler angles with Polulu AltIMU-10 v5 sensor
 * and dissplays the results in pitch-roll-yaw format.
 * 
 * We are using the source code of the arduino programme Pololu MinIMU-9 + Arduino AHRS 
 * (Attitude and Heading Reference System) that can be found here;
 * https://github.com/pololu/minimu-9-ahrs-arduino
 * (Copyright (c) 2011 Pololu Corporation.
 * http://www.pololu.com/)
 */
 


altIMU sensor;

void setup() {
  Serial.begin(115200);
  sensor = altIMU(1);




}


void loop() {

  /*repeatedly creating triggers for the sensor and printing
    the distance value*/

  float res = sensor.getAngleAroundYAxis();

  Serial.print("angulo alrededor eje Y: ");
  Serial.print(res);
  Serial.print("\n");
  
}



