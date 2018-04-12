#include "hcsr04.h" //include the declaration for this class

void HCSR04::initiate() {
  dist=0;

  //we set up PIN22 (PA0) as Trigger 
  DDRA = _BV(DDA0); //we set PA0 to output mode
  PORTA = 0;
  
  //we set up PIN2 (PE4) as Echo 
  DDRE &= ~_BV(DDE4); //we set PE4 to input mode
  setupTimer();
  setupInterruption();
}

float HCSR04::getDistance(){
  // we generate trigger
  PORTA |= _BV(PA0);
  _delay_ms(20);
  PORTA &= ~_BV(PA0);
  
  // first interruption - rising edge of the echo signal
  while(!(0b00010000 & EIFR));  // we are checking if the interruption flag has been changed
  EIFR = _BV(INTF4);            // we are reseting the flag by setting it with logical 1
  TCCR1B = 0;
  TCNT1=0;                      // stop the timer and then clear it
  TCCR1B = _BV(CS11);
  
  // second interruption - falling edge of the echo signal
  while(!(0b00010000 & EIFR));  // we are checking if the interruption flag has been changed
  EIFR = _BV(INTF4);            // we are reseting the flag by setting it with logical 1
  unsigned int useconds = TCNT1*0.5f; //  we are reading the value of the timer
  dist=(float)(useconds)/58.0f;       //  transforming the result to seconds

  return dist;
}

void HCSR04::setupTimer() {
  TCCR1A = 0;
  TCCR1B = _BV(CS11);           //we set the prescaler to 8, the lowest possible to get maximum accuracy. 
}

void HCSR04::setupInterruption() {
  cli();
  EICRB = _BV(ISC40);         //interrupt triggered by both falling and raising edge
  //EIMSK = _BV(INT4);        // we are not enabling interrupts this time because we are going to check
                              // for changes the interrupt flag instead
  sei();                      //enable global interrupts
}


