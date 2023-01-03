// C++ code
//
/*

18.06.2022 
Author: SUBHA

*/



unsigned char PWM_BIT_BANG=0;

struct PIN {
  volatile uint8_t * PORT;
  uint8_t PIN;
};

struct {
  volatile uint8_t * PORT;
  uint8_t PIN;
}SERVOCONF;

ISR(TIMER1_COMPA_vect){

PWM_BIT_BANG = 1;
*SERVOCONF.PORT |= (1<<SERVOCONF.PIN);
  
}

ISR(TIMER1_COMPB_vect){

PWM_BIT_BANG = 0;
*SERVOCONF.PORT &= ~(1<<SERVOCONF.PIN);
  
}


void servoBegin(){
  TCCR1A=0;
  TCCR1B=0;

  TCCR1B |= (1<<CS12) | (1<<WGM12) | (1<<WGM13); // Prescaler = 256
  ICR1 =  1249;
  OCR1A = 0;
  TIMSK1 |= (1<<OCIE1A) | (1<<OCIE1B);
  sei();
}


void servoEnd(){
  TCCR1A=0;
  TCCR1B=0;
  TIMSK1=0;
  cli();
}




void ServoWrite(struct PIN PINCONF,int pos){
  SERVOCONF.PORT = PINCONF.PORT;
  SERVOCONF.PIN = PINCONF.PIN;
  OCR1B = map(pos,0,180,25,153);
}



int main(){

  
  DDRB = (1<<PB5) | (1<<PB4) | (1<<PB3);
  servoBegin();
  
  struct PIN MyServo = {&PORTB , PB5};
  struct PIN MyServo2 = {&PORTB , PB4};
  struct PIN MyServo3 = {&PORTB , PB3};

  int x=0;

  
  for(x=0;x<180;x++){
    ServoWrite(MyServo,x);
  }

  _delay_ms(500);
  
  while(x>=0){
    ServoWrite(MyServo,x);
    _delay_ms(10);
    x--;
  }

  _delay_ms(100);
  //servoEnd();
  
  while(1){
    
    for(x=0;x<180;x++){
      ServoWrite(MyServo,x);
      _delay_ms(10);
    }

    
    ServoWrite(MyServo2,90);
    _delay_ms(100);
    ServoWrite(MyServo3,180);
    _delay_ms(100);
  }

}
