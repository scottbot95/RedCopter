PROGMEM const byte rxPins[6] = {
  RX_PIN_PITCH, RX_PIN_THROTTLE, RX_PIN_ROLL, RX_PIN_AUX1, RX_PIN_AUX2, RX_PIN_YAW};
volatile byte rxState[4] = {0, 0, 0, 0};
volatile int rxPrev[6] = {0, 0, 0, 0, 0, 0};
volatile int rxPrevVal0=LKNOB_WMIN+10, rxPrevVal1=YAW_WMIN+10;

void rxInit() {
  for(byte i=0;i<6;i++){
    pinMode(pgm_read_byte(&rxPins[i]),INPUT); 
    digitalWrite(pgm_read_byte(&rxPins[i]),HIGH);
  }
  PCICR |= (1 << PCIE0); 
  PCMSK0 |= (1 << PCINT1)|(1 << PCINT2)|(1 << PCINT3)|(1 << PCINT4); 
  sei();
  attachInterrupt(RX_INT_AUX2,rxGoesHigh0,CHANGE);
  attachInterrupt(RX_INT_YAW,rxGoesHigh1,CHANGE);
}

void rxGoesHigh0(){
  if (digitalRead(rxPins[4])) {
    rxPrev[4]=micros();
  } else {
    rxVal[4]=micros()-rxPrev[4];
  }
}
void rxGoesHigh1(){
  if (digitalRead(rxPins[5])) {
    rxPrev[5]=micros();
  } else {
    rxVal[5]=micros()-rxPrev[5];
  }
}

ISR(PCINT0_vect) 
{ 
  for(byte i=0;i<4;i++){
    byte rxtemp=digitalRead(pgm_read_byte(&rxPins[i]));
    if((rxState[i] == 0) & (rxtemp==1)){
      rxPrev[i]=micros();
      rxState[i]=1;
    }
    else if((rxState[i] == 1) & (rxtemp==0)){
      rxVal[i]=micros()-rxPrev[i];
      rxState[i]=0;
    }
  }
}


