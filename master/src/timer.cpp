// Description: This file implements functions that utilize the timers
//----------------------------------------------------------------------//

#include "timer.h"
#include <Arduino.h>

/* Initialize timer 1, you should not turn the timer on here. You will need to
* use CTC mode
*/
void initTimer1(){
  //CTC TIMER 1 AT /64
  TCCR1A &= ~(1 << WGM10 | 1 << WGM11);
  TCCR1B |= (1 << WGM12);
  TCCR1B &= ~(1 << WGM13);

  TCCR1B &= ~(1 << CS10 | 1 << CS11 | 1 << CS12);

}



void delayMs(unsigned int delay)
{
  TCNT1H = 0x00;
  TCNT1L = 0x00;

  OCR1A = (250 * delay) - 1;

  // Turn on the timer
  TCCR1B |= (1 << CS10 | 1 << CS11);
  TCCR1B &= ~(1 << CS12);

  while (!(TIFR1 & (1 << OCF1A)));

  // Clear the OCF1A flag
  TIFR1 |= (1 << OCF1A);
  // Turn off the timer
  TCCR1B &= ~(1 << CS10 | 1 << CS11 | 1 << CS12);
}

/* This delays the program an amount specified by unsigned int delay.
*/
void delayUs(unsigned int delay){

  // clear the timer
  TCNT1H = 0x00;
  TCNT1L = 0x00;
  // calculate the TOP value and assign it to OCR1A

  OCR1A = (16 * delay) - 1;

  // Turn on the timer
  TCCR1B |= (1 << CS10);
  TCCR1B &= ~(1 << CS11 | 1 << CS12); // 0 div

  while (!(TIFR1 & (1 << OCF1A)));

  // Clear the OCF1A flag
  TIFR1 |= (1 << OCF1A);
  // Turn off the timer
  TCCR1B &= ~(1 << CS10 | 1 << CS11 | 1 << CS12);
}


void delayS(unsigned int delay){
  int milli_sec = delay * 1000;

  for(int i = 0; i < milli_sec; i=i+200)
    delayMs(200);

}
