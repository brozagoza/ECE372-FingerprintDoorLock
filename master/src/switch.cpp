#include "switch.h"
#include <Arduino.h>

void initSwitch(){
  PORTK |= (1 << PORTK0 | 1 << PORTK1);
  DDRK &= ~(1 << DDK0 | 1 << DDK1);
}
