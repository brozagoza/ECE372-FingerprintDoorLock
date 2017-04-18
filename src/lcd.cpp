// Description: This file implements functions to operate an LCD
//----------------------------------------------------------------------//

#include "lcd.h"
#include "timer.h"
#include <util/delay.h>
#include <Arduino.h>

static unsigned int count = 0;
/* Asserts Logic High on Enable Pin
*/
void enableHigh(){
PORTB |= (1 << PORTB6);
}

/* Asserts Logic Low on Enable Pin
*/
void enableLow(){
PORTB &= ~(1 << PORTB6);
}

/* Asserts Logic Low on RS Pin
*/
void rsLow(){
PORTB &= ~(1 << PORTB7);
}

/* Asserts Logic High on RS Pin
*/
void rsHigh(){
PORTB |= (1 << PORTB7);
}

/* Initializes Tri-state for LCD pins and calls initialization procedure.
* This function is made so that it's possible to test initLCDPins separately
* from initLCDProcedure which will likely be necessary.
*/
void initLCD(){
  initLCDPins();
  initLCDProcedure();
}

/* Asserts enableHigh, delays, asserts enableLow, then delays again according to
* the specification on the data sheet. This function is meant to be called after
* the data pins have been set at appopriate values. This effectively "sends" the
* bits that are set on the data pins.
*/

void sendCommand(){
enableHigh();
delayUs(1);
enableLow();
delayUs(1);

}

/* Initializes all pins related to the LCD to be outputs
*/
void initLCDPins(){
DDRA |= (1 << DDA0 | 1 << DDA1 | 1 << DDA2 | 1 << DDA3);
DDRB |= (1 << DDB6 | 1 << DDB7 );//| 1 << DDB5 | 1 << DDB4); //ddb1 = rs ddb3 = enable

}

/* 1. Assert bits to pins connected to DB[7:4] on the LCD screen
*      a. data is an unsigned char which has 8 bits. Therefore, you
*         need assign the bottom 4 bits of "data" to the appropriate bits in
*         PORTA
*  2. This is a "data" signal, meaning RS should be High
*  3. send the command!
*  4. delay the provided number in MICROseconds.
*/
void clearscreen()
{
  writeCommandWithUsDelay(0x00, 0);
  writeCommandWithUsDelay(0x01, 2000);
  count = 0;
}

void writeDataWithUsDelay(unsigned char data, unsigned int delay){


  if(count == 15)
  {
    count = 64;
  }
  unsigned char tempH = count >> 4;
  unsigned char tempL = count << 4;
  tempL = tempL / 16;
  count ++;
  rsLow();
  delayUs(1);

  PORTA |= (1 << PORTA3);
  PORTA &= ~(1 << PORTA2 | 1 << PORTA1 | 1 << PORTA0);
  PORTA |= tempH;

  sendCommand();
  PORTA &= ~(1 << PORTA2 | 1 << PORTA1 | 1 << PORTA0 | 1 << PORTA3);
  PORTA |= tempL;
  sendCommand();
  delayUs(delay);

  rsHigh();
  delayUs(1);
  unsigned char cempH = data >> 4;
  unsigned char cempL = data << 4;
  cempL = cempL / 16;

  PORTA &= ~(1 << PORTA2 | 1 << PORTA1 | 1 << PORTA0 | 1 << PORTA3);
  PORTA |= cempH;
  sendCommand();
  PORTA &= ~(1 << PORTA2 | 1 << PORTA1 | 1 << PORTA0 | 1 << PORTA3);
  PORTA |= cempL;
  sendCommand();
  delayUs(delay);
}

/* Similar to writeDataWithUsDelay except that now all eight bits of command are
* sent.
* 1. Assert bits to pins connected to DB[7:4] on the LCD screen
*       a. unlike writeDataWithUsDelay, you need to send the TOP four bits of
*          "command" first. These should be assigned to appropriate bits in PORTA
* 2. This is a command signal, meaning RS should be Low
* 3. send the command!
* 4. Now set the lower four bits of command to appropriate bits in PORTA
* 5. Send the command!
* 6. delay the provided number in MICROseconds.
*/
void writeCommandWithUsDelay(unsigned char command, unsigned int delay){
  rsLow();
  delayUs(1);
  unsigned char tempH = command >> 4;
  unsigned char tempL = command << 4;
    tempL = tempL / 16;
    PORTA &= ~(1 << PORTA2 | 1 << PORTA1 | 1 << PORTA0 | 1 << PORTA3);
  if(command > 15)
  {
    PORTA |= tempH;
    sendCommand();
    PORTA &= ~(1 << PORTA2 | 1 << PORTA1 | 1 << PORTA0 | 1 << PORTA3);
    PORTA |= tempL;
  }
  else{
    PORTA |= command;
  }
  sendCommand();
  if(delay > 4000) // gives some le-way for higher delays
  {
    delayUs(delay - 4000);
    delayUs(4000);
  }
  else
  {
      delayUs(delay);
  }
}

/* Writes a character to the LCD screen. This is done by giving a character
* such as 'G' to writeCommandWithUsDelay and delaying according to the datasheet.
*/
void writeCharacter(unsigned char cem)
{
  writeDataWithUsDelay(cem, 50);
}

/* Writes a provided string such as "Hello!" to the LCD screen. You should
* remember that a c string always ends with the '\0' character
*/
void writeString(const char *string){
  clearscreen();
  int stringindex = 0;
  count = 0;

  while(string[stringindex] != '\0')
  {
   writeCharacter(string[stringindex]);
    stringindex++;
  }

}

/* This is the procedure outline on the LCD datasheet page 4 out of 9.
* This should be the last function you write as it largely depends on all other
* functions working.
*/
void initLCDProcedure(){

  // Delay 15 milliseconds
  rsLow();
  enableLow();
    delayMs(20);
  writeCommandWithUsDelay(0x03, 5000);
 writeCommandWithUsDelay(0x03, 150);
  writeCommandWithUsDelay(0x32, 50);
  // Function set in the command table
writeCommandWithUsDelay(0x28, 50);
  // Display off in the command table
writeCommandWithUsDelay(0x0,0);
  writeCommandWithUsDelay(0x8,50);
writeCommandWithUsDelay(0x0,0);
writeCommandWithUsDelay(0x1, 2000);
  // Entry Mode Set in the command table.
  writeCommandWithUsDelay(0x0,0);
  writeCommandWithUsDelay(0x6,50);

  // Display ON/OFF Control in the command table. (Yes, this is not specified),
  // in the data sheet, but you have to do it to get this to work. Yay datasheets!)
  writeCommandWithUsDelay(0x0,0);

  writeCommandWithUsDelay(0xC,50);
}
