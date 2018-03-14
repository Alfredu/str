#ifndef HCSR04_H
#define HCSR04_H

#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/*
 * This class measures and returns the distance in cm measured with 
 * the sensor HC-SR04.
 *
 * To do so, we use the 16 bit Timer1 and the Logic-level change interruption 4.
 * We decided to use Timer1 to maximize timing and distance measuring accuracy.
 * We decided to use interruption 4 to avoid clashing with other functions such as i2c.
 */

class HCSR04 { 
public:
	HCSR04();
	~HCSR04();

  /*
   * in this function We generate high signal for trigger for >10ms so that 
   * the sensor would generate echo signal that will enable
   * an interruption
   */
  void createTrigger();
	float returnResult();
  boolean isResultReady();
private:
  setupTimer();
  setupInterruption();
  boolean measuring_dist;
  float dist;
  boolean result_ready;
};

#endif
