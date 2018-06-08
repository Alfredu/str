/*working variables*/
#include "PIDball.h"

/*
 * Class used to calculate the goal position of the servo 
 * given the current distance of the ball using PID controller.
 */

 /*
  * Function used to set the goal distance of the ball. To be used
  * in the Setup() part of the program.
  */
void PIDball::setGoal(double setP) {
  setPoints = setP;
  lastTime = millis();  
  errSum = 0;
  lastErr = 0;
}

/*
 * Function used to compute the goal position (output) of the servo
 * given the current distance of the ball (input) and 
 * the current position of the servo (currentMotVal)
 */
double PIDball::compute(double input, double currentMotVal)
{
  /*How long since we last calculated*/
  unsigned long now = millis();
  double timeChange = (double)(now - lastTime);
  /*Compute all the working error variables*/
  double error = setPoints - input;
  errSum += (error * timeChange);
  double dErr = (error - lastErr) / timeChange;
  /*Compute PID Output*/
  double output = (kp * error) + (ki * errSum) + (kd * dErr);
  /*Remember some variables for next time*/
  lastErr = error;
  lastTime = now;
  return transformValue(currentMotVal, output);
}
void PIDball::setTunings(double Kp, double Ki, double Kd)
{
  kp = Kp;
  ki = Ki;
  kd = Kd;
}

/*
 * Function to transform the value calculated with PID controller (value)
 * to the value that is appriopriate to write in servo.
 */
double PIDball::transformValue(double currentMotVal, double value) 
{
  double x = 5;
  if (value>x) value=x;
  if (value<-x) value = -x;
  double ret = currentMotVal+value;
  if (ret>85) return 85;
  if (ret<60) return 60;
  return ret;
}


