
#include <avr/io.h>
#include <avr/interrupt.h>

#define TRUE 1
#define FALSE 0

volatile unsigned long currentMillis = 0;
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
  
  DDRD |= 0x04;
  //PORTD |= 0x04;
  configInterrupt();
  //DDRD |= 0x04;
  // INT0 = PD2
  DDRD &= 0xFB;
  // INT1 = PD3 =. &11110111
  DDRD &= 0xF7;
  //PD6 output
  //DDRD |= 0x40;// 0100 0000 PF6
  DDRD |= (1 << 6);
  // PD4 output
  DDRD |= (1 << 4);

  timer0();
  timer2();

  sei();
  lastInterrupt = 0;

  while(TRUE){
    PORTD &= 0xFF;
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
  OCR0A = 128;//243; //250
  TCCR0A &= 0x00;
  // PWM Update of OCRx at bottom isTCCR0A |= 0xF3; update top is 1
  TCCR0A |= 0xF3;
  TCCR0B &= 0xF0;
  // Prescale 5 is 1024, 1 is no prescaling
  // 328p has  20Mhz (5*10^-8 s per cycle)
  TCCR0B |= 0x04;
}

void timer2(){
  TCCR2A &= 0x00;
  TCCR2B &= 0x00;
  // Prescale 1024 -> 20Mhz / 1024 = 42.2 us * 24 = 1012,8
  TCCR2B |= 0x07;
  OCR2A = 24;
  // Interrupt when match on A
  //TIMSK2 |= (1 << 1) OCIE2A; 
  // Enable interrupt
  //TIMSK2 |= (1 << TOIE2);
  TIMSK2 |= (1 << OCIE2A);

}

ISR(TIMER2_COMPA_vect){
  currentMillis++;
  ledTime++;
  if(ledTime > 38){
    //togglePd4();
    ledTime = 0;
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
  //turnOffSwitches();
  togglePd4();
  unsigned long diffTime = currentMillis - lastInterrupt;
  if (lastInterrupt == 0 || diffTime > 30){
    //togglePd4();
    if(OCR0A > 0){
      OCR0A--;
    }
    lastInterrupt = currentMillis;
  }
  //turnOnSwitches();
}

ISR(INT1_vect){
  //turnOffSwitches();
  unsigned long diffTime = currentMillis - lastInterrupt;
  if (lastInterrupt == 0 || diffTime > 30){
    if(OCR0A < 0xFF){
      //OCR0A++;
    }
    lastInterrupt = currentMillis;
  }
  //turnOnSwitches();
}

void turnOnSwitches(){
  EIMSK |= 0x03;
}

void turnOffSwitches(){
  EIMSK &= 0x00;
}