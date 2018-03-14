#include "hcsr04.h" //include the declaration for this class

HCSR04::HCSR04() {
  measuring_dist=0;
  dist=0;
  result_ready=false;

  //we set up PIN22 (PA0) as Trigger 
  DDRA = _BV(DDA0); //we set PA0 to output mode
  PORTA = 0;
  //we set up PIN2 (PE4) as Echo 
  DDRE &= ~_BV(DDE4); //we set PE4 to input mode
  this->setupTimer();
  this->setupInterruption();
}

void HCSR04::createTrigger(){
  result_ready=false;
  PORTA |= _BV(PA0);
  _delay_ms(20);
  PORTA &= ~_BV(PA0);
}

boolean HCSR04::isResultReady() {
  return result_ready;
}

float HCSR04::returnResult() {
  return dist;
}

void HCSR04::setupTimer() {
  TCCR1A = 0;
  TCCR1B = _BV(CS11); //we set the prescaler to 8, the lowest possible to get maximum accuracy.
  
}

void HCSR04::setupInterruption() {
  cli();
  EICRB = _BV(ISC40); //interrupt triggered by both falling and raising edge
  EIMSK = _BV(INT4); //we enable interrupts by INT4 (pin2)
  sei(); //enable global interrupts
}


//Interrupt4 Service Routine
ISR(INT4_vect) {
  
  if (!measuring_dist) {
      TCCR1B = 0;
      TCNT1=0;//stop the timer and then clearing it
      TCCR1B = _BV(CS11); 
      measuring_dist=1;
    } else {
      //0.5 comes from the CPU freq settings and the prescaler.
      unsigned int useconds = TCNT1*0.5f;
      dist=(float)(useconds)/58.0f;
      measuring_dist=0;
      result_ready=true;
    }
}
