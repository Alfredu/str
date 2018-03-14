#include <avr/io.h>
#include <avr/interrupt.h>

/*
 * This program displays the distance in cm measured with 
 * the sensor HC-SR04.
 *
 * To do so, we use the 16 bit Timer1 and the Logic-level change interruption 4.
 * We decided to use Timer1 to maximize timing and distance measuring accuracy.
 * We decided to use interruption 4 to avoid clashing with other functions such as i2c.
 */
 
HCSR04 sensor();

void setup() {
  
  sensor

}


void loop() {
  /*repeatedly creating triggers for the sensor and printing
    the distance value*/
  
  create_trigger();
  _delay_ms(50);
  Serial.println(dist);
}



}





