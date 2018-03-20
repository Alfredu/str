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

  sensor.I2C_Init();
  //delay(1500);
  
  sensor.Accel_Init();
  sensor.Compass_Init();
  sensor.Gyro_Init();

  delay(20);

  sensor.calculateOffset();


}


void loop() {

  /*repeatedly creating triggers for the sensor and printing
    the distance value*/
  sensor.readValue();
  Serial.print("ANG:");
  Serial.print(ToDeg(sensor.getRoll()));
  Serial.print(",");
  Serial.print(ToDeg(sensor.getPitch()));
  Serial.print(",");
  Serial.print(ToDeg(sensor.getYaw()));
  Serial.print("\n");
}



