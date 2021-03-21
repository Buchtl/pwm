
#include <avr/io.h>
#include <avr/interrupt.h>

#define TRUE 1
#define FALSE 0


int main (void){
  
  DDRD |= 0x04;
  //PORTD |= 0x04;
  //ISC01 und ISC00 loeschen und setzen fuer any edge (1) oder rising (11)
  EICRA &= 0xFC;
  // Rising Edge INT0 is 11 for ISC01 and ISC00 which is bit 0 and 1 > 0011
  EICRA |= 0x03,
  // Rising Edge INT1 is 11 for ISC11 and ISC10 which is bit 2 and 3 > 1100
  EICRA |= 0x0C;
  // enable INT0 and INT1
  EIMSK |= 0x03;
  
  //DDRD |= 0x04;
  // INT0 = PD2
  DDRD &= 0xFB;
  // INT1 = PD3 =. &11110111
  DDRD &= 0xF7;
  //PD6 output
  //DDRD |= 0x40;// 0100 0000 PF6
  DDRD |= (1 << 6);
  // PD5 output
  //DDRD |= (1 << 5);
  //OCR0B = 128;
  OCR0A = 243; //250
  TCCR0A &= 0x00;
  // PWM Update of OCRx at bottom isTCCR0A |= 0xF3; update top is 1
  TCCR0A |= 0xF3;
  TCCR0B &= 0xF0;
  // Prescale 5 is 1024, 1 is no prescaling
  // 328p has  20Mhz (5*10^-8 s per cycle)
  TCCR0B |= 0x01;

  //sei();

  while(TRUE){
    PORTD &= 0xFF;
 }
}

ISR(INT0_vect){
  if(OCR0A > 0){
    OCR0A--;
  }
}
ISR(INT1_vect){
  if(OCR0A < 0xFF){
    OCR0A++;
  }
}
  
