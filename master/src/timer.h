// Description: This file describes functions used by the timer
//----------------------------------------------------------------------//
#ifndef TIMER_H
#define TIMER_H
#include <avr/io.h>
void initTimer1();
void initTimer3();

void delayUs(unsigned int x); // greatest can do is 4 ms
void delayMs(unsigned int x); // greatest can do is 200 ms
void delayS(unsigned int x);
#endif
