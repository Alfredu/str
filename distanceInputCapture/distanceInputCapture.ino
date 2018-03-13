#include <avr/io.h>
#include <avr/interrupt.h>

/*
 * This program displays the distance in cm measured with 
 * the sensor HC-SR04.
 *
 * To do so, we use the 16 bit Timer1 and the input capture mode of this timer
 * We decided to use Timer1 to maximize timing and distance measuring accuracy.
 * 
 */
boolean measuring_dist = 0;
float dist = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //we set up DIGPIN22 (PA0) as Trigger 
  DDRA = _BV(DDA0); //we set PA0 to output mode
  PORTA = 0;
  //we set up DIGPIN49 (PL0) as Echo 
  DDRL &= ~_BV(DDL0); //we set PL0 to input mode
  
  setup_timer();
  setup_input_interruption();

}

void setup_timer() {
  TCCR4A = 0;
  TCCR4B = _BV(CS41)  | _BV(ICES4); //we set the prescaler to 8, the lowest possible to get maximum accuracy.
  
}

void setup_input_interruption() {
  cli();
  TIMSK4 = _BV(ICIE4);  //input capture interrupt enabled
  sei(); //enable global interrupts
}

//We generate high signal for trigger for >10ms so that 
//the sensor would generate echo signal that will enable
//an interruption
void create_trigger(){
  PORTA |= _BV(PA0);
  _delay_ms(20);
  PORTA &= ~_BV(PA0);
}
void loop() {
  /*repeatedly creating triggers for the sensor and printing
    the distance value*/
  create_trigger();
  _delay_ms(50);
  Serial.println(dist);
}


//Interrupt4 Service Routine
ISR(TIMER4_CAPT_vect) {
  if (!measuring_dist) {
    TCCR4B = 0;
    TCNT4=0;//stop the timer and then clearing it
    TCCR4B = _BV(CS41); //set falling edge of input capture  
    measuring_dist=1;
  } else {
    //0.5 comes from the CPU freq settings and the prescaler.
    unsigned int timer_value = ICR4;  //first the low value then the high is copied
                                                    //to the temp register.
    TCCR4B = 0;
    TCNT4=0;//stop the timer and then clearing it
    TCCR4B = _BV(CS41) | _BV(ICES4); //set raising
    
    unsigned int useconds = timer_value*0.5f;
    dist=(float)(useconds)/58.0f;
    measuring_dist=0;
  }
}





