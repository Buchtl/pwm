
#include <avr/io.h>
#include <avr/interrupt.h>

#define TRUE 1
#define FALSE 0
#define INT_BREAK 10

volatile unsigned long currentTime = 0;
volatile unsigned long lastInterrupt = 0;
volatile unsigned long ledTime = 0;


void timer0();
void timer2();
void pd4(int value);
void togglePd4();
void turnOnSwitches();
void turnOffSwitches();
void configInterrupt();

int main (void){
  
  configInterrupt();
  // INT0 = PD2 INT1 = PD3
  DDRD &= ~((1 << PIND2) | (1 << PIND3));
  //PD6 output
  DDRD |= (1 << PIND6);
  DDRD |= (1 << PIND4);

  timer0();
  timer2();

  sei();

  while(TRUE){
    // Sleep Mode Control Register
    // in SCMR SM2..0 written 0 -> idle
    //sleep instead
    SMCR &= ~((1 << SM2) | (1 << SM1) | (1 << SM0));
 }
}

void configInterrupt(){
    //ISC01 und ISC00 loeschen und setzen fuer any edge (1) oder rising (11)
  EICRA &= 0xFC;
  // Rising Edge INT0 is 11 for ISC01 and ISC00 which is bit 0 and 1 > 0011
  //EICRA |= 0x03,
  // Rising Edge INT1 is 11 for ISC11 and ISC10 which is bit 2 and 3 > 1100
  //EICRA |= 0x0C;
  EICRA |= 0x0F;
  // enable INT0 and INT1
  EIMSK |= 0x03;
}

void timer0(){
  //OCR0B = 128;
  OCR0A = 248;//243; //250
  TCCR0A &= 0x00;
  TCCR0A |=  (1 << WGM00) | (1 << WGM02);//(1 << WGM00) | (1 << WGM02);
  //TCCR0A |= (1 << COM0A0) | (1 << COM0A1);
  TCCR0A |= (1 << COM0A1) | (1 << COM0A0);
  TCCR0B &= 0x00;
  //TCCR0B |= (1 << CS02) | (1 << CS00);// | (1 << WGM02);
  TCCR0B |= (1 << CS00);
  // PWM Update of OCRx at bottom isTCCR0A |= 0xF3; update top is 1
  // Prescale 5 is 1024, 1 is no prescaling
  // 328p has  20Mhz (5*10^-8 s per cycle)
}

void timer2(){
  TCCR2A &= 0x00;
  // Clear Timer on Compare match (CTC)
  TCCR2A |= (1 << WGM01);
  TCCR2B &= 0x00;
  // Prescale 1024 -> 20Mhz -> 50ns cycle -> prescale 1024 = 51,2 us * 19 = 972,8us
  TCCR2B |= 0x07;
  OCR2A = 19;
  TIMSK2 |= (1 << OCIE2A);

}

ISR(TIMER2_COMPA_vect){
  currentTime++;
  if(currentTime - ledTime > 100){
    togglePd4();
    ledTime = currentTime;
  }
}

void togglePd4(){
  if((PORTD & (1 << PIND4)) != 0){
      pd4(FALSE);
    }else{
      pd4(TRUE);
    } 
}

void pd4(int value){
  if(value != 1){
    PORTD &= 0xEF;
  } else {
    PORTD |= (1 << PIND4);
  }
}


ISR(INT0_vect){
  turnOffSwitches();
  int delayed = currentTime > (lastInterrupt + INT_BREAK) || lastInterrupt == 0;
  if(delayed == 1 && OCR0A > 0 && OCR0A <= 0xFF){
    OCR0A--;
    lastInterrupt = currentTime;
  }
  turnOnSwitches();
}

ISR(INT1_vect){
  turnOffSwitches();
  int delayed = currentTime  > (lastInterrupt + INT_BREAK) || lastInterrupt == 0;
  if(delayed == 1 && OCR0A >= 0 && OCR0A < 0xFF){
    OCR0A++;
    lastInterrupt = currentTime;
  }
  turnOnSwitches();
}

void turnOnSwitches(){
  EIMSK |= 0x03;
}

void turnOffSwitches(){
  EIMSK &= 0x00;
}