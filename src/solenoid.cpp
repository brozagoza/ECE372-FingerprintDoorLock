// Description: This file implements functions to open or close solenoid
//----------------------------------------------------------------------//

#include "solenoid.h"
#include <util/delay.h>
#include <Arduino.h>

void solenoid_unlock(){
  PORTH &= ~(1 << PORTH3);
}

void solenoid_lock(){
  PORTH |= (1 << PORTH3);
}
