// Author:
// Net ID:
// Date:
// Assignment:
//
// Description:
//----------------------------------------------------------------------//

#ifndef PWM_H
#define PWM_H
#include <avr/io.h>

void initPWM(double hz);
void setPWM(double freq);
void turn_off_PWM();
void beep_PWM();


#endif
