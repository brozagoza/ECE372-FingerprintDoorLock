/*

*/
#include "macros.h"

volatile stateType state = LOGIN;

// SPI Variables
volatile unsigned char recv_val = 0;
volatile spiStateType spi_state = send;

// Hardware setup - FPS connected to:
//	  digital pin 4(arduino rx, fps tx)
//	  digital pin 5(arduino tx - 560ohm resistor fps tx - 1000ohm resistor - ground)
//		this brings the 5v tx line down to about 3.2v so we dont fry our fps


FPS_GT511C3 fps(10, 11);
unsigned int count = 0;
bool displayflag = true;
bool logtodelete = false;
bool safetodelete = false;
bool singledelete = false;
bool lockstatus = true;

/* ======================================================
*  Setup Function
*
* Initializes all components that will be used in our project.
========================================================*/
void setup()
{
	Serial.begin(BAUD_RATE);
  initTimer1();
	initSwitch();
  initSPIMaster();
  initLCD();
	sei();

	delayMs(100);
	fps.Open();
	solenoid_lock();
	fps.SetLED(true);
}


void loop()
{
	switch(state)
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case LOGIN:
			login_function();
			break;
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
		case ENROLL_SEL:
		if(displayflag)
{
		writeString("MENUE:          1.ENROLL A ID...");
		fps.SetLED(false);
		displayflag = false;
}
		if(!(PINK & (1 << PINK0)))
		{
			displayflag = true;
			state = CHOOSE_DELETE_SEL;
			delayMs(500);
			//delayMs(200);
		}
		else if(!(PINK & (1 << PINK1)))
		{
			state = ENROLL;
			displayflag = true;
			delayMs(500);
			//delayMs(200);
		}
		break;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case ENROLL:
		fps.SetLED(true);
		Enroll();
		state = ENROLL_SEL;
		break;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case DELETE_SEL:
		if(displayflag)
{
		writeString("MENUE:          3.CLEAR ALL ID's...");
				fps.SetLED(false);
		displayflag = false;
}
		if(!(PINK & (1 << PINK0)))
		{
			state = LOGIN_SEL;
					displayflag = true;
			delayMs(500);
			//delayMs(200);
		}
		else if(!(PINK & (1 << PINK1)))
		{
			state = DELETE;
			displayflag = true;
			delayMs(500);
			//delayMs(200);
		}
		break;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case CHOOSE_DELETE_SEL:
		if(displayflag)
{
		writeString("MENUE:          2.CLEAR A ID...");
				fps.SetLED(false);
		displayflag = false;
}
		if(!(PINK & (1 << PINK0)))
		{
			state = DELETE_SEL;
					displayflag = true;
			delayMs(500);
			//delayMs(200);
		}
		else if(!(PINK & (1 << PINK1)))
		{
			state = DELETE;
			singledelete = true;
			displayflag = true;
			delayMs(500);
			//delayMs(200);
		}
		break;
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		case LOGIN_SEL:
		if(displayflag)
{
		writeString("MENUE:          4.LOG IN...");
				fps.SetLED(false);
		displayflag = false;
}
		if(!(PINK & (1 << PINK0)))
		{
			state = ENROLL_SEL;
					displayflag = true;
			delayMs(500);
			//delayMs(200);
		}
		else if(!(PINK & (1 << PINK1)))
		{
			state = LOGIN;
			displayflag = true;
			delayMs(500);
			//delayMs(200);
		}
		break;
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case CHOOSEDELETE:
		displayflag = true;
		int temp;
		temp = findid();
		fps.DeleteID(temp);
		writeString("ID SUCCEFULLY DELETED");
		delayS(2);
		safetodelete = false;
		logtodelete = false;
		state = CHOOSE_DELETE_SEL;
		break;
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case DELETE:
		if(displayflag)
{
		writeString("LOG IN TO DELETE ENTRIES...");
				fps.SetLED(false);
		displayflag = true;
		delayS(3);
		logtodelete = true;
		state = LOGIN;
}

if(safetodelete)
{
	fps.DeleteAll();
	writeString("DELETED ALL ENTRIES...");
		delayS(3);
			safetodelete = false;
			logtodelete = false;
			displayflag = true;
			state = DELETE_SEL;
}
if(!(PINK & (1 << PINK0)))
{
	state = LOGIN_SEL;
			displayflag = true;
	delayMs(500);
	//delayMs(200);
}
		break;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}

}



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
	strcpy(t_s, "PLEASE PRESS    FINGER TO ENROLL");

	// enroll
	writeString(t_s);//Serial.print("Press finger to Enroll #");
	//Serial.println(enrollid);
	while(fps.IsPressFinger() == false) delay(100);
	bool bret = fps.CaptureFinger(true);
	int iret = 0;
	if (bret != false)
	{
		writeString(" REMOVE FINGER");//Serial.println("Remove finger");
		fps.Enroll1();
		while(fps.IsPressFinger() == true) delay(100);
					writeString("   PRESS SAME     FINGER AGAIN");//Serial.print("Enrolling Failed with error code:");
		while(fps.IsPressFinger() == false) delay(100);
		bret = fps.CaptureFinger(true);
		if (bret != false)
		{
			writeString(" REMOVE FINGER");//Serial.println("Remove finger");
			fps.Enroll2();
			while(fps.IsPressFinger() == true) delay(100);
						writeString("   PRESS SAME     FINGER AGAIN");//Serial.print("Enrolling Failed with error code:");
			while(fps.IsPressFinger() == false) delay(100);
			bret = fps.CaptureFinger(true);
			if (bret != false)
			{
				writeString(" REMOVE FINGER");//Serial.println("Remove finger");
				iret = fps.Enroll3();
        delay(1000);
				if (iret == 0)
				{
							writeString("ENROLL SUCCEFULL");//Serial.println("Enrolling Successfull");
					_delay_ms(2000);
				}
				else
				{
					writeString("ENROLL FAILED");//Serial.print("Enrolling Failed with error code:");
					_delay_ms(2000);
					//Serial.println(iret);
				}
			}
			else writeString("Failed on 3rd");//Serial.println("Failed to capture third finger");
		}
		else writeString("Failed on 2nd");//Serial.println("Failed to capture second finger");
	}
	else writeString("Failed on 1st");//Serial.println("Failed to capture first finger");
}

// need to do manual transmission for this to work

int findid()
{
	int countt = 1;

	while(1)
	{
	  if(displayflag)
	  {
			switch(countt)
			{
				case 1:
				{
					writeString("DELETE ID (1)?");
        	displayflag = false;
					break;
				}
				case 2:
				{
					writeString("DELETE ID (2)?");
          displayflag = false;
					break;
				}
				case 3:
				{
					writeString("DELETE ID (3)?");
          displayflag = false;
					break;
				}
				case 4:
				{
					writeString("DELETE ID (4)?");
          displayflag = false;
					break;
				}
				case 5:
				{
					writeString("DELETE ID (5)?");
          displayflag = false;
					break;
				}
				case 6:
				{
					writeString("DELETE ID (6)?");
					displayflag = false;
					break;
				}
			case 7:
			{
			writeString("DELETE ID (7)?");
						displayflag = false;
			break;
			}
			case 8:
			{
			writeString("DELETE ID (8)?");
						displayflag = false;
			break;
			}
			case 9:
			{
			writeString("DELETE ID (9)?");
						displayflag = false;
			break;
			}
			case 10:
			{
			writeString("DELETE ID (10)?");
						displayflag = false;
			break;
			}
      case 11:
      {
        displayflag = false;
        countt = 0;
        break;
      }
			}
	      }
	 				if(!(PINK & (1 << PINK0)))
	 					{
	            Serial.println("NEXT BUTTON IS PRESSED");
	 						countt++;
	 								displayflag = true;
	 						delayMs(500);
	 						//delayMs(200);
	 					}
	 					if(!(PINK & (1 << PINK1)))
	 					{
	                  Serial.println("select BUTTON IS PRESSED");
	            delayMs(500);
							displayflag = true;
	 						return countt - 1;
	 					}
	}
}




void login_function(){
	if (displayflag)
		fps.SetLED(true);

	if (fps.IsPressFinger()){
		fps.CaptureFinger(false);
		int id = fps.Identify1_N();

		if (id < ID_THRESH){
		  writeString(" *ID VERIFIED*");
			count = 0;	// reset count
			delayS(2);

			if(logtodelete){
				displayflag = false; // THIS FOR DELETING ALL
				safetodelete = true;

				if(singledelete)
						state = CHOOSEDELETE;
				else
					state = DELETE;

				singledelete = false;
			} // end logtodelete
			else{
        if(lockstatus){
					//SOLONOID UNLOCK HERE
	        solenoid_unlock();
					writeString(" DOOR UNLOCKED");
					delayS(2);
        	lockstatus = false;
        }
        else
        {
					//SOLONOID LOCK HERE
          solenoid_lock();
          writeString("  DOOR LOCKED");
					delayS(2);
        	lockstatus = true;
        }
        displayflag = true;
			} // end GRAND ELSE

		} // END ID THRESH IF
		else{
			count++;
			writeString("  *ID FAILED*");
	    //solenoid_lock();
			delayS(2);
	    displayflag = true;
		}
	} // end IS PRESS FINGER IF


	else {
		if (count > 2) {
			 writeString("****LOCKDOWN********************");
			 solenoid_lockdown();
	     delayS(5);

			count = 0;
			state = LOGIN;
			return;
		} // end count if

    if(displayflag)
    {
      if(lockstatus) {
        writeString("PLEASE PRESS    FINGER TO UNLOCK");
      }
      else {
        writeString("PLEASE PRESS    FINGER TO LOCK");
			}
      displayflag = false;
    } // end displayflag



	}
	if(!(PINK & (1 << PINK0))) // next button
	{
		state = ENROLL_SEL;
		displayflag = true;
		delayMs(500);
  //  delayMs(200);
  }

} // end login_function




//// THE NEW YOGESH CODE
void solenoid_unlock(){
  //Access Granted
  sendData(ACCEPT);
  while(spi_state != receive_something);
  delayMs(10);
}

void solenoid_lock(){
  //Access Denied
  sendData(DENIED);
  while(spi_state != receive_something);
  delayMs(10);
}

void solenoid_reset(){
  //Reset
sendData(RESET);
while(spi_state != receive_something);
delayMs(10);
}

void solenoid_lockdown(){
  // LOCKDOWN
  sendData(LOCK_DOWN);
  while(spi_state != receive_something);
  delayMs(10);
}

//SPI Serial Transmission Complete
ISR(SPI_STC_vect){
  recv_val = SPDR;
  spi_state = receive_something;
  PORTB |= (1 << PORTB0); //put SS high
}
