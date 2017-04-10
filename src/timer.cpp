// Description: This file implements functions that utilize the timers
//----------------------------------------------------------------------//

#include "timer.h"
#include <Arduino.h>
#include <avr/io.h>

/* Initialize timer 1, you should not turn the timer on here. You will need to
* use CTC mode
*/
void initTimer1(){
  DDRH |= (1 << DDH3);
 // set the timer mode to be "CTC"
 TCCR1A &= ~(1 << WGM10 | 1 << WGM11);
 TCCR1B &= ~(1 << WGM13);
 TCCR1B |= (1 << WGM12);

 // Turn on the timer // this is 8...
 TCCR1B &= ~(1 << CS10 | 1 << CS12);
 TCCR1B |= (1 << CS11);
 PORTH |= (1 << PORTH3);
}

/* This delays the program an amount specified by unsigned int delay.
*/
void delayUs(unsigned int delay){

 unsigned int ticks = 2 * delay;  // got 2 from Nick
 // clear the timer
 TCNT1H = 0; // hi
 TCNT1L = 0; // lo

 // calculate the TOP value and assign it to OCR1A
 OCR1AH = ticks >> 8; // "top" 8 bits of delay
 OCR1AL = ticks & 0x00FF; // "lower" 8 bits of delay

 TCCR1B &= ~(1 << CS10 | 1 << CS12);
 TCCR1B |= (1 << CS11);

 // Do nothing while the OCF1A flag is not up
 while (!(TIFR1 & (1 << OCF1A)));

 // Clear the OCF1A flag
 TIFR1 |= (1 << OCF1A);

 // Turn off the timer
 TCCR1B &= ~(1 << CS10 | 1 << CS12 | 1 << CS11);
}
