//Slave

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t rx = 0;

void
SPI_Init ()
{
  PORTC.DIR = 0 b01000000;  // MOSI, SCK, SS outputs MISO input
  SPIC.CTRL = 0 b01000000;  // spi master, spi mode 0
  SPIC.INTCTRL = 0 b00000011;   // assign high priority to spic interrupts
  PMIC.CTRL = 0 b00000100;  // enable high priority interrupts
}

void
SPI_Trans (uint8_t dat)
{
  SPIC_DATA = dat;
  while (!(SPIC_STATUS & 0x80)) // wait for transmit complete
    {
      PORTF_OUTSET = 0x02;  //LED at PF1 will glow
      //_delay_ms (500);
    }
  //PORTF_OUTCLR = 0x02;
  //_delay_ms (500);
}

uint8_t
SPI_Rec ()
{
  PORTF_OUTSET = 0x01;      //LED at PF0 will glow
  //_delay_ms (500);
// grab received byte
  while (!(SPIC_STATUS & 0x80));    //wait till reception is complete
  uint8_t dat = SPIC_DATA;
  //PORTF_OUTCLR = 0x01;
  //_delay_ms (500);
  return dat;
}

int
main (void)
{
  PORTF_DIRSET = 0xFF;
  cli ();
  sei ();           // global interrupts
  SPI_Init ();

//SPIC.DATA = rx; //transmit

  while (!(SPIC_STATUS & 0x80));    //wait till reception is complete
      rx = SPIC_DATA;
  
  while (1)
    {
//_delay_ms(1000);
// grab received byte
      //_delay_ms (500);
      //PORTF_OUTCLR = 0x01;
      //_delay_ms (500);
      //if (rx == 4)
   // PORTF_OUTSET = 0x80;
//_delay_ms(1000);
//PORTF_OUTCLR=0x01;
    SPI_Trans(rx);
    }
}

ISR (SPIC_INT_vect)
{
  rx = SPI_Rec ();
  SPI_Trans (rx);
  //rx = SPIC.DATA;     // store received
  //SPIC.DATA = rx;     // send back
//rx = SPI_Rec();
  //_delay_ms (1000);
  PORTF_OUT |= 1 << (rx);
  //_delay_ms (1000);
  //PORTF_OUTCLR = 1 << (rx);
}
