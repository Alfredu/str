void setup() {
  // put your setup code here, to run once:

}

void setup_timer() {
  TCCR1B = _BV(CS11) | _BV(CS10); //we set the prescaler to 64
  
}

void setup_interruption() {
  EICRA = _BV(ISC00);
  EIMSK = _BV(INT0);
  sei();
}

ISR(INT0_vect) {
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
