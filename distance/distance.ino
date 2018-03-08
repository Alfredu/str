#include <avr/io.h>
#include <avr/interrupt.h>

boolean measuring_dist = 0;
float dist = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  setup_timer();
  setup_interruption();

}

void setup_timer() {
  TCCR1B = _BV(CS11) | _BV(CS10); //we set the prescaler to 64
  
}

void setup_interruption() {
  EICRA = _BV(ISC00); //interrupt triggered by both falling and raising edge
  EIMSK = _BV(INT0); //we enable interrupts by INT0 (pin21)
  sei();
}

ISR(INT0_vect) {
  
  if (!measuring_dist) {
    /* this part is probably totally wrong. Marta */
    TCNT1=0;
    measuring_dist=1;
  } else {
    dist=TCNT1/58;
    measuring_dist=0;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(dist);
  Serial.print("\n");

}
