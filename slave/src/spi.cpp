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
  //PBO (SS) an input
  DDRB |= (1 << DDB3);
  DDRB &= ~(1 << DDB0 | 1 << DDB1 | 1 << DDB2);

  //enable spi interrupt, enable SPI, master mode
  //SPI Clock rate = 1 MHZ
  SPCR |= (1 << SPIE | 1 << SPE | 1 << CPOL | 1 << CPHA | 1 << SPR0);
  SPCR &= ~(1 << MSTR);
}
