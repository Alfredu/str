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
boolean measuring_dist = 0;
float dist = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  setup_timer();
  setup_interruption();
  //we set up PIN22 (PA0) as Trigger 
  DDRA = _BV(DDA0); //we set PA0 to output mode
  PORTA = 0;
  //we set up PIN2 (PE4) as Echo 
  DDRE &= ~_BV(DDE4); //we set PE4 to input mode

}

void setup_timer() {
  TCCR1A = 0;
  TCCR1B = _BV(CS11); //we set the prescaler to 8, the lowest possible to get maximum accuracy.
  
}

void setup_interruption() {
  cli();
  EICRB = _BV(ISC40); //interrupt triggered by both falling and raising edge
  EIMSK = _BV(INT4); //we enable interrupts by INT4 (pin2)
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

    
  }
}





