#ifndef PIDBALL_H_
#define PIDBALL_H_

#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>



class PIDball { 
public:
  void setGoal(double);
	double compute(double, double);
  void setTunings(double Kp, double Ki, double Kd);
  
private:
  unsigned long lastTime;
  double setPoints;
  double errSum, lastErr;
  double kp, ki, kd;
  double transformValue(double, double);

};

#endif
