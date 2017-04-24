#ifndef MACROS_H
#define MACROS_H

// Contains macros so there are no magic numbers ;)

#define BAUD_RATE 9600
#define ID_THRESH 200

#include "switch.h"
#include "spi.h"
#include "FPS_GT511C3.h"
#include "SoftwareSerial.h"
#include "lcd.h"
#include "timer.h"
#include <Arduino.h>
#include <avr/io.h>

typedef enum stateType_enum{
  CHOOSE_DELETE_SEL, ENROLL, ENROLL_SEL, LOGIN_SEL, LOGIN, DELETE_SEL, DELETE, CHOOSEDELETE
} stateType;

void Enroll();
int findid();

void login_function();


void solenoid_lock();
void solenoid_unlock();
void solenoid_reset();
void solenoid_lockdown();

#endif MACROS_H
