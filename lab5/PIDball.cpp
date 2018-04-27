/*working variables*/


void PIDball::setGoal(double setP) {
  setPoints = setP;
  lastTime = millis();  
  errSum = 0;
  lastErr = 0;
}

double PIDball::Compute(double Input)
{
  /*How long since we last calculated*/
  unsigned long now = millis();
  double timeChange = (double)(now - lastTime);
  /*Compute all the working error variables*/
  double error = Setpoint - Input;
  errSum += (error * timeChange);
  double dErr = (error - lastErr) / timeChange;
  /*Compute PID Output*/
  Output = kp * error + ki * errSum + kd * dErr;
  /*Remember some variables for next time*/
  lastErr = error;
  lastTime = now;
  return Output;
}
void PIDball::SetTunings(double Kp, double Ki, double Kd)
{
  kp = Kp;
  ki = Ki;
  kd = Kd;
}


