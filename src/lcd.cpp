// Description: This file implements functions to operate an LCD
//----------------------------------------------------------------------//

#include "lcd.h"
#include "timer.h"
#include <util/delay.h>
#include <Arduino.h>


/* Asserts Logic High on Enable Pin
*/
void enableHigh(){
  PORTB |= (1 << PORTB6); // sets PB5 high
}

/* Asserts Logic Low on Enable Pin
*/
void enableLow(){
  PORTB &= ~(1 << PORTB6);  // set PB5 low
}

/* Asserts Logic Low on RS Pin
*/
void rsLow(){
  PORTB &= ~(1 << PORTB7);  // set PB4 low
}

/* Asserts Logic High on RS Pin
*/
void rsHigh(){
  PORTB |= (1 << PORTB7); // set PB4 high
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
  delayUs(1); // i dont know this value pls help
  enableLow();
  delayUs(1);
}

/* Initializes all pins related to the LCD to be outputs
*/
void initLCDPins(){
  // sets my RS and Enable pin as outputs
  DDRB |= (1 << DDB6 | 1 << DDB7);

  // sets Data pins as outputs
  DDRA |= (1 << DDA0 | 1 << DDA1 | 1 << DDA2 | 1 << DDA3);

}

/* 1. Assert bits to pins connected to DB[7:4] on the LCD screen
*      a. data is an unsigned char which has 8 bits. Therefore, you
*         need assign the bottom 4 bits of "data" to the appropriate bits in
*         PORTA
*  2. This is a "data" signal, meaning RS should be High
*  3. send the command!
*  4. delay the provided number in MICROseconds.
*/
void writeDataWithUsDelay(unsigned char data, unsigned int delay){
  PORTA = (0x0F & data); // assigns bottom 4 bits of data to the DB's
  //rsHigh();  // set RS low homie
  rsLow();
  sendCommand();  // send the command!!!!!!!!!! OMG SEND IT NOW DO IT
  delayUs(delay);
}

/* Similar to writeDataWithUsDelay except that now all eight bits of command are
* sent.
* 1. Assert bits to pins connected to DB[7:4] on the LCD screen
*       a. unlike writeDataWithUsDelay, you need to send the TOP four bits of
*          "command" first. These should be assigned to appropriate bits in PORTA
* 2. This is a command signal, meaning RS should be low
* 3. send the command!
* 4. Now set the lower four bits of command to appropriate bits in PORTA
* 5. Send the command!
* 6. delay the provided number in MICROseconds.
*/
void writeCommandWithUsDelay(unsigned char command, unsigned int delay){
  PORTA = (0xF0 & command) >> 4;  // assigns top 4 bits to the DB's
  rsHigh();
  sendCommand();  // all right all right i'll send it!!!
  PORTA = (0x0F & command); // assigns lower 4 bits to the DB's
  sendCommand();
  delayUs(delay);
}

/* Writes a character to the LCD screen. This is done by giving a character
* such as 'G' to writeCommandWithUsDelay and delaying according to the datasheet.
*/
void writeCharacter(unsigned char character){
  writeCommandWithUsDelay(character, 46);
}

/* Writes a provided string such as "Hello!" to the LCD screen. You should
* remember that a c string always ends with the '\0' character
*/
void writeString(const char *string){
  char *p;
  unsigned int position = 0b10000000;

  for (p = string; *p != '\0'; p++)
  {
    writeDataWithUsDelay((0xF0 & position) >> 4, 0); // set DDRAM
    writeDataWithUsDelay(0x0F & position, 40);


    writeCharacter(*p); // write character
    position++; // increment position
  } // end for loop

} // end writeString

/* This is the procedure outline on the LCD datasheet page 4 out of 9.
* This should be the last function you write as it largely depends on all other
* functions working.
*/
void initLCDProcedure(){
  // Delay 15 milliseconds
  delayUs(15000);
  // Write 0b0011 to DB[7:4] and delay 4100 microseconds
  writeDataWithUsDelay(0b0011, 4100);
  // Write 0b0011 to DB[7:4] and delay 100 microseconds
  writeDataWithUsDelay(0b0011, 100);
  // The data sheet does not make this clear, but at this point you are issuing
  // commands in two sets of four bits. You must delay after each command
  // (which is the second set of four bits) an amount specified on page 3 of
  // the data sheet.
  // write 0b0011 to DB[7:4] and no delay
  writeDataWithUsDelay(0b0011, 0);
  // write 0b0010 to DB[7:4] and delay.
  //writeCommandWithUsDelay(0b00110010, 40);  // 0011 0010 40micro delay
  writeDataWithUsDelay(0b0010, 40);
  // Function set in the command table
  //writeCommandWithUsDelay(0b00101000, 40);  // 0010 1000 40micro delay
  writeDataWithUsDelay(0b0010, 0); writeDataWithUsDelay(0b1000, 40);

  // Display off in the command table

  //writeCommandWithUsDelay(0b00001000, 40);  // 0000 1000 40micro delay
  writeDataWithUsDelay(0b0000,0); writeDataWithUsDelay(0b1000, 40);

  // Clear display in the command table. Remember the delay is longer!!!

  //writeCommandWithUsDelay(0b00000001, 82);  // 0000 0001 82micro delay
  writeDataWithUsDelay(0b0000, 0); writeDataWithUsDelay(0b0001, 1640);

  // Entry Mode Set in the command table.
  //writeCommandWithUsDelay(0b00000101, 40);  // 0000 01D1 40micro delay
  writeDataWithUsDelay(0b0000, 0); writeDataWithUsDelay(0b0110, 40);

  // Display ON/OFF Control in the command table. (Yes, this is not specified),
  // in the data sheet, but you have to do it to get this to work. Yay datasheets!)
  //writeCommandWithUsDelay(0b00001111, 40);  // display on
  writeDataWithUsDelay(0b0000, 0); writeDataWithUsDelay(0b1100, 40);

}
