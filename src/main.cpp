/*
	FPS_Enroll.ino - Library example for controlling the GT-511C3 Finger Print Scanner (FPS)
	Created by Josh Hawley, July 23rd 2013
	Licensed for non-commercial use, must include this license message
	basically, Feel free to hack away at it, but just give me credit for my work =)
	TLDR; Wil Wheaton's Law
	This sketch will attempt to identify a previously enrolled fingerprint.
*/

#include "FPS_GT511C3.h"
#include "SoftwareSerial.h"
#include "lcd.h"
#include "timer.h"
#include "macros.h"
#include "solenoid.h"
#include "switch.h"
#include <Arduino.h>
#include <avr/io.h>

// Hardware setup - FPS connected to:
//	  digital pin 4(arduino rx, fps tx)
//	  digital pin 5(arduino tx - 560ohm resistor fps tx - 1000ohm resistor - ground)
//		this brings the 5v tx line down to about 3.2v so we dont fry our fps

FPS_GT511C3 fps(10, 11);
unsigned int count = 0;

// PORTH |= (1 << PORTH3);
void setup()
{
	Serial.begin(BAUD_RATE);
  initTimer1();
  initLCD();
	sei();

	delay(100);
	fps.Open();

	fps.DeleteAll(); // clear all at first

	fps.SetLED(true);
	Enroll();
  delay(1000);
}


void loop()
{

	// Identify fingerprint test
	if (fps.IsPressFinger())
	{
		fps.CaptureFinger(false);
		int id = fps.Identify1_N();
		if (id < ID_THRESH)
		{
			writeString("Verified ID");//Serial.print("Verified ID:");
			//Serial.println(id);

      delay(100);
		}
		else
		{
			count++;
			writeString("Finger not found");//Serial.println("Finger not found");

		}
	}
	else
	{
		if (count > 2) {
			writeString("LOVE LOCKDOWN");
			while (true);
		}
		writeString("Please press finger");//Serial.println("Please press finger");
		solenoid_lock();

	}
	delay(1000);
}


// ISR(PCINT0_vect)
// {
//   writeString("Interrupt");
//
// } // end ISR


/*
*	Code to enroll another finger print ya feel dawg
*/
void Enroll()
{
	// Enroll test

	// find open enroll id
	int enrollid = 0;
	bool usedid = true;
	while (usedid == true)
	{
		usedid = fps.CheckEnrolled(enrollid);
		if (usedid==true) enrollid++;
	}
	fps.EnrollStart(enrollid);
	const char t_s[80];
	strcpy(t_s, "Press finger to Enroll at ID");

	// enroll
	writeString(t_s);//Serial.print("Press finger to Enroll #");
	//Serial.println(enrollid);
	while(fps.IsPressFinger() == false) delay(100);
	bool bret = fps.CaptureFinger(true);
	int iret = 0;
	if (bret != false)
	{
		writeString("Remove finger");//Serial.println("Remove finger");
		fps.Enroll1();
		while(fps.IsPressFinger() == true) delay(100);
		writeString("Press same finger again");//Serial.println("Press same finger again");
		while(fps.IsPressFinger() == false) delay(100);
		bret = fps.CaptureFinger(true);
		if (bret != false)
		{
			writeString("Remove finger");//Serial.println("Remove finger");
			fps.Enroll2();
			while(fps.IsPressFinger() == true) delay(100);
			writeString("Press same finger yet again");//Serial.println("Press same finger yet again");
			while(fps.IsPressFinger() == false) delay(100);
			bret = fps.CaptureFinger(true);
			if (bret != false)
			{
				writeString("Remove finger");//Serial.println("Remove finger");
				iret = fps.Enroll3();
        delay(1000);
				if (iret == 0)
				{
					writeString("Enrolling Successfull");//Serial.println("Enrolling Successfull");
				}
				else
				{
					writeString("Enrolling FAILED");//Serial.print("Enrolling Failed with error code:");
					//Serial.println(iret);
				}
			}
			else writeString("Failed on 3rd");//Serial.println("Failed to capture third finger");
		}
		else writeString("Failed on 2nd");//Serial.println("Failed to capture second finger");
	}
	else writeString("Failed on 1st");//Serial.println("Failed to capture first finger");
}
