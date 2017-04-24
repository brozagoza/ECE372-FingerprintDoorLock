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

#define ACCEPT 0x47
#define DENIED 0x44
#define RESET 0x52
#define LOCK_DOWN 0x4C
#include "timer.h"

typedef enum spiStateType_enum{
  send, receive_something
}spiStateType;

void initSPIMaster();
void sendData(unsigned char data);


#endif
