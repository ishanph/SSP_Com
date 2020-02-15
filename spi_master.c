//Master

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint_8t tx = 4;

void
SPI_Init ()
{
  PORTD.DIR = 0 b10110000;  // MOSI, SCK, SS outputs MISO input
  PORTD.OUTSET = 0 b00010000;   // sets SS pin high
  SPID.CTRL = 0 b01010000;  // spi master, spi mode 0
  SPID.INTCTRL = 0 b00000011;   // assign high priority to spid interrupts
  PMIC.CTRL = 0 b00000100;
}

void
SPI_Trans (uint8_t dat)
{
  SPID_DATA = dat;
  while (!(SPID_STATUS & 0x80)) // wait for transmit complete
    {
      PORTF_OUTSET = 0x02;  //LED at PF1 will glow
 //     _delay_ms (500);
    }
//  PORTF_OUTCLR = 0x02;
//  _delay_ms (500);
}

uint8_t
SPI_Rec ()
{
  PORTF_OUTSET = 0x01;      //LED at PF1 will glow
//  _delay_ms (500);
  uint8_t dat = SPID_DATA;  // grab received byte
  while (!(SPID_STATUS & 0x80));    //wait till reception is complete
//  PORTF_OUTCLR = 0x01;
 // _delay_ms (500);
  return dat;
}

int
main (void)
{
  PORTF_DIRSET = 0xFF;
  SPI_Init ();

  PORTD.OUTCLR = 0 b00010000;   //pull ss low
  sei ();           //global interrupts

//tx = SPID.DATA; //recieve

 // while (1)
    {
      SPI_Trans (tx);
  //    _delay_ms (10000);
      PORTF_OUT |= 1 << tx;
    }
}

ISR (SPID_INT_vect)
{
  SPI_Rec(tx);
  tx++;
  SPI_Trans(tx); //store recieved
//SPID.DATA = tx + 1; //increment and send back
  //_delay_ms (1500);
  PORTF_OUT = 0 b10000000;
  PORTF_OUT |= 1 << tx;
  //_delay_ms (1500);
  //PORTF_OUT = 0x00;
}
