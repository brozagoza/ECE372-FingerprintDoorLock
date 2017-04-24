// Author:
// Net ID:
// Date:
// Assignment:
//
// Description:
//----------------------------------------------------------------------//
#include "pwm.h"
#include "timer.h"
#include <Arduino.h>

// Inits the PWM
void initPWM(double hz){
  // Configure TCCR1A and TCCR1B // fast pwm timing // max resolution is 16 bits = OCR1A
  TCCR1A |= (1 << WGM10 | 1 << WGM11 | 1 << COM1A0);

  TCCR1A &= ~(1 << COM1A1);
  TCCR1B |= ( 1 << WGM13 | 1 << CS10);
  TCCR1B &= ~(1 << WGM12 | 1 << CS12 | 1 << CS11);
  // set the duty cycles


  int temp = (16000000 / (2 * hz));
//  temp = 16000000 / (2*hz);
  OCR1A = temp;

  // set the pin as output (this must be done last)
  DDRB |= (1 << DDB5);
}

void beep_PWM()
{
  for(int x = 24000; x > 2000; x -= 22) // x = frequency you want buzzer to play
  {
    initPWM(x);
    delayUs(100);
  }
}

// Turns off the PWM
void turn_off_PWM()
{
  DDRB &= ~(1 << DDB5);
}
