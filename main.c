
#include <avr/io.h>
#include <avr/interrupt.h>

#define TRUE 1
#define FALSE 0

typedef unsigned char BYTE;

#define ADDR_OFFSET 0x20

#define _MMIO_BYTE2(mem_addr) ((volatile BYTE *)( mem_addr))
#define _REG_ADDR2(io_addr) _MMIO_BYTE2(io_addr + ADDR_OFFSET)

#define ADDR_TCNT0 _REG_ADDR2(0x26)
//#define TCNT0 *ADDR_TCNT0
#define ADDR_DDRD _REG_ADDR2(0x0A)
//#define DDRD *ADDR_DDRD
#define ADDR_PORTD _REG_ADDR2(0x0B)
//#define PORTD *ADDR_PORTD
#define ADDR_PIND _REG_ADDR2(0x09)
//#define PIND *ADDR_PIND
#define ADDR_OCR0A _REG_ADDR2(0x27)
//#define OCR0A *ADDR_OCR0A
#define ADDR_OCR0B _REG_ADDR2(0x28)
//#define OCR0B *ADDR_OCR0B
//#define TIMSK0 *_REG_ADDR2(0x6E)
//#define TIFR0 *_REG_ADDR2(0x15)
//#define TCCR0A *_REG_ADDR2(0x24)
//#define TCCR0B *_REG_ADDR2(0x25)

// SREG – AVR Status Register
//Bit 7 – I: Global Interrupt Enable
#define ADDR_SREG _REG_ADDR2(0x3F)
//#define SREG *ADDR_SREG

// External interrupt control register
#define ADDR_EICRA _REG_ADDR2(0x69)
//#define EICRA *ADDR_EICRA
// External interrupt mask register
#define ADDR_EIMSK _REG_ADDR2(0x1D)
//#define EIMSK *ADDR_EIMSK
// EIFR – External Interrupt Flag Register -> bit one is 1 if interrupt occured
#define ADDR_EIFR _REG_ADDR2(0x1C)
//#define EIFR *ADDR_EIFR
/*
Bit 1 - PCIE1: Pin Change Interrupt Enable 1
When the PCIE1 bit is set (one) and the I-bit in the status register (SREG) is set (one), pin change interrupt 1 is enabled. Any
change on any enabled PCINT14..8 pin will cause an interrupt. The corresponding interrupt of pin change interrupt request is
executed from the PCI1 interrupt vector. PCINT14..8 pins are enabled individually by the PCMSK1 register.
*/
// PCICR – Pin Change Interrupt Control Register
/*
When the PCIE0 bit is set (one) and the I-bit in the status register (SREG) is set (one), pin change interrupt 0 is enabled. Any
change on any enabled PCINT7..0 pin will cause an interrupt. The corresponding interrupt of pin change interrupt request is
executed from the PCI0 interrupt vector. PCINT7..0 pins are enabled individually by the PCMSK0 register
PCIFR – Pin Change Interrupt Flag Register
*/
#define ADDR_PCICR _REG_ADDR2(0x68)
//#define PCICR *ADDR_PCICR

#define ADDR_INT0 _REG_ADDR2(0x002)
//#define INT0 *ADDR_INT0

int globalchange = 0;

int main (void){
  
  DDRD |= 0x04;
  //PORTD |= 0x04;
  //ISC01 und ISC00 loeschen und setzen fuer any edge (1) oder rising (11)
  EICRA &= 0xFC;
  EICRA |= 0x03,
  EIMSK |= 0x01;
  
  //DDRD |= 0x04;
  // INT0 = PD2
  DDRD &= 0xFB;
  DDRD |= 0x40;
  DDRD &= 0xEF;
  //PORTD |= 0x80;
  OCR0A = 250; //250
  TCCR0A &= 0x00;
  TCCR0A |= 0xF3;
  TCCR0B &= 0xF0;
  TCCR0B |= 0x05;

  sei();

  while(TRUE){
    PORTD &= 0xFF;
    //if((EIFR&0x01) != 0){
      //OCR0A = 100;
    //}
    //OCR0A = 100;
    //EIFR |= 0x01;
   //asm volatile("nop");
   //if((PIND & 0x10) != 0 ){
   //  OCR0A = 200;
  // }else{
  //   OCR0A = 128;
  // }
 }
}

ISR(INT0_vect){
  if(globalchange == 0){
    OCR0A = 128;
    globalchange = 1;
  }else{
    OCR0A = 200;
    globalchange = 0;
  }
}
  
