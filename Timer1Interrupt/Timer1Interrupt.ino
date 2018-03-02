#include <avr/io.h>
#include <avr/interrupt.h>

void setup() {
  cli();
  DDRB |= _BV(DDB7);
  //prescaler 256
  TCCR1A = 0;        // set entire TCCR1A register to 0
  TCCR1B = 0b00000100; //prescaler a 256

  // initialize counter
  //2^16 - 62500
  TCNT1 = 3036;


  // enable overflow interrupt
  TIMSK1 |= (1 << TOIE1);

  // enable global interrupts
  
  sei();
}

void loop() {
  // put your main code here, to run repeatedly:
}


ISR(TIMER1_OVF_vect)
{
    PINB = _BV(PORTB7);
}


