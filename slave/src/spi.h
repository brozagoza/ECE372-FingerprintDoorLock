// Author:
// Net ID:
// Date:
// Assignment:
//
// Description:
//----------------------------------------------------------------------//

#ifndef SPI_H
#define SPI_H
#include <avr/io.h>

typedef enum stateType_enum{
  wait, access_granted, access_denied, security_lockdown
}statetype;

void initSPIMaster();

#endif
