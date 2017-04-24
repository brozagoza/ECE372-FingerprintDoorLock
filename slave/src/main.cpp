// Author:         Garrett Vanhoy
// Net ID:         gvanhoy
// Date:           16 March 2017
// Assignment:     Lab 3 Part 2
//
// Description: This file contains a programmatic overall description of the
// program. It should never contain assignments to special function registers
// for the exception key one-line code such as checking the state of the pin.
//
// Requirements:
//----------------------------------------------------------------------//

#include <avr/interrupt.h>
#include <avr/io.h>
#include <Arduino.h>
#include "timer.h"
#include "pwm.h"
#include "solenoid.h"
#include "spi.h"

volatile unsigned char recv_val = 0;
volatile statetype access = wait;

int main(){
  Serial.begin(9600);
  initSPIMaster();
  initTimer1();
  sei();
  DDRH |= (1 << DDH3);

  while(1){
    //beep_PWM();
    switch (access) {
      case wait:
      Serial.println(recv_val);
        turn_off_PWM();
        solenoid_lock();
        break;
      case access_granted:
        Serial.println(recv_val);
        //open the lock
        solenoid_unlock();
        break;
      case access_denied:
        solenoid_lock();
        Serial.println(recv_val);
        break;
      case security_lockdown:
        //Set the buzzer
        beep_PWM();
        solenoid_lock();
        break;
    }

  }
  return 0;
}

//SPI Serial Transmission Complete
ISR(SPI_STC_vect){
  recv_val = SPDR;
  if(recv_val == 0x47){
    access = access_granted;
  }
  else if (recv_val == 0x44){
    access = access_denied;
  }
  else if (recv_val == 0x4C){
    access = security_lockdown;
  }
  else if (recv_val == 0x52){
    access = wait;
  }
}
