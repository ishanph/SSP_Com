
#define F_CPU 2000000
#include <avr/io.h>
#include <util/delay.h>
char data = 'a';

int
main (void)
{

  while (1)
    {
      PORTC.DIRSET = 1 << 3;	//sets tx
      PORTC.DIRCLR = 1 << 2;	//sets rx

      USARTC0_BAUDCTRLA = 0 b00001100;	//sets baudrate
      USARTC0_BAUDCTRLB = 0 b00000000;	//seta bscale

      USARTC0_CTRLB = 1 << 3;	//enable transmission
      USARTC0_CTRLC = 0 b00000011;	//set char length

      while (!(USARTC0_STATUS & 0 b00100000));	//waits for empty register
      USARTC0_DATA = data;
      _delay_ms (100);

    }
