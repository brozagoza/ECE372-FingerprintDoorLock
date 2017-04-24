// Author:
// Net ID:
// Date:
// Assignment:
//
// Description:
//----------------------------------------------------------------------//

#include <avr/io.h>
#include "spi.h"

void initSPIMaster(){
  DDRB |= (1 << DDB0 | 1 << DDB1 | 1 << DDB2);
  DDRB &= ~(1 << DDB3);

  //enable spi interrupt, enable SPI, master mode
  //SPI Clock rate = 1 MHZ
  SPCR |= (1 << SPIE | 1 << SPE | 1 << MSTR | 1 << CPOL | 1 << CPHA | 1 << SPR0);

  //Set SS to high by default
  PORTB |= (1 << PORTB0);
}


void sendData(unsigned char data){
  SPDR = data;
  PORTB &= ~(1 << PORTB0);
}
