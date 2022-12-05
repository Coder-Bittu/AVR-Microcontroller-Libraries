

#define F_CPU 16000000

volatile char cmd[100];

void USART_begin(long baud){
  
  UCSR0A=0;
  UCSR0B=0;
  UCSR0C=0;
  
  UCSR0B = (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0) ; // Transmitter has been enabled
  
  //UCSR0C = (1<<UMSEL00) | (1<<UMSEL01); // ASynchronous Mode
  UCSR0C = UCSR0C&(~(1<<UPM00)) | UCSR0C&(~(1<<UPM01)) | (1<<USBS0); // No parity , 1 stop bit
  UCSR0C = (1<<UCSZ00) | (1<<UCSZ01); // Set USART for 8 bit data transmission
  UBRR0= ((F_CPU/16)/baud)-1;

}


void USART_stopTX(){
  UCSR0B &= ~(1<<TXEN0);
}

void USART_stopRX(){
  UCSR0B &= ~(1<<RXEN0);
}


void USART_end(){
  UCSR0A=0;
  UCSR0B=0;
  UCSR0C=0;
}


void USART_putc(unsigned char data){ // To transmit single charecter (8 bit data)
  UDR0=data;
  while(!((UCSR0A>>5)&1));
}


void USART_print(char* data){ // To transmit String
  int c=0;
  while(data[c]!='\0'){
   USART_putc(data[c]);
    c++;
  }
}


void USART_println(char* data){
  USART_print(data);
  USART_putc('\n');
}


char USART_getc(){
  while( !((UCSR0A>>RXC0)&1) ){}  // if RXCn is zero , wait.
   return UDR0;
}



char USART_gets(char * data){
  char ch;
  int x = 0;
  while( 1 ){
    ch = USART_getc();
    if(ch=='\n' || ch=='\0' || ch=='\r'){
      break;
    }
    data[x++]=ch;
  }
}


ISR(USART_RX_vect){
  //memset(cmd,0,sizeof(cmd));
  USART_println("RX Interrupt");
  volatile char ch;
  int x = 0;
  while( 1 ){
    ch = USART_getc();
    if(ch=='\n' || ch=='\0' || ch=='\r'){
      break;
    }
    cmd[x++]=ch;
  }
 
}





int main(){

  
  
  
  USART_begin(9600);
  sei();
  
  int c=0;
  char str[100]="ATMega328P USART in pure C!";
  char input[100]={0};
  char i;
  
  USART_println(str);
  USART_print(">> ");
  
 
  
  while(1){
    //USART_gets(input);
    USART_println(cmd);
    _delay_ms(1000);
  }
  
  
}
