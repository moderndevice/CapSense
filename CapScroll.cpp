/*
 * CapScroll.cpp rev01 - Capacitive Scroll Wheel Library for 'duino / Wiring
 * Paul Bagder 2009-2011
 *  9-27-2011 <paul@moderndevice.com> http://opensource.org/licenses/mit-license.php
 */

#include "CapScroll.h"


// include core Wiring API

#include "pins_arduino.h"
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#include "pins_arduino.h"
#include "WConstants.h"
#endif



// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

CapScroll::CapScroll(uint8_t pinA, uint8_t pinB, uint8_t pinC){
		
	// get pin mapping and port for pinA	
	aBit =  digitalPinToBitMask(pinA);              // get pinA's ports and bitmask
	aPort = digitalPinToPort(pinA);
	aOut = portOutputRegister(aPort);               // get pointer to output register   
	aIn  = portInputRegister(aPort);				// get pointer to input register 
	aReg = portModeRegister(aPort);                 // get pointer to pin direction register
	
	// get pin mapping and port for pinB	
	bBit =  digitalPinToBitMask(pinB);              // get pinB's ports and bitmask
	bPort = digitalPinToPort(pinB);
	bOut = portOutputRegister(bPort);               // get pointer to output register   
	bIn  = portInputRegister(bPort);				// get pointer to input register 
	bReg = portModeRegister(bPort);                 // get pointer to pin direction register
	
	// get pin mapping and port for pinC	
	cBit =  digitalPinToBitMask(pinC);              // get pinC's ports and bitmask
	cPort = digitalPinToPort(pinC);
	cOut = portOutputRegister(cPort);               // get pointer to output register   
	cIn  = portInputRegister(cPort);				// get pointer to input register 
	cReg = portModeRegister(cPort);                 // get pointer to pin direction register
	
	 // this Arduino core function appears not to work	
	if (aPort == NOT_A_PORT || bPort == NOT_A_PORT || cPort == NOT_A_PORT) { 
		Serial.println("bad pin number");
		return;
	}
	
calibrateFlag = 0; 
	
}

/*********************************************************************************/

/* Here's the general overview of the method
 Read the scroll wheel as three sliders - swapping send and receive pins.
 The first time through save values in a baseline value. This should be
 with sensor untouched. There is a one second delay to allow users to get
 away from the sensor on powerup.
 
 This slider-read technique leads to six values. The raw values have the baseline
 subtracted from them. They are then merged into 3 vectors associated with each wire.
 Vector arithmetic is then used to sum vectors and resolve angle and touch values.
 */

ScrollWheelData CapScroll::readScroll(unsigned int samples){
	
	uint8_t sBit;   // declare variables for send pin's ports and bitmask
	uint8_t sPort;
	volatile uint8_t *sReg;
	volatile uint8_t *sOut;
	volatile uint8_t *sIn;
	
	uint8_t rBit;   // declare variables for receive pin's ports and bitmask
	uint8_t rPort;
	volatile uint8_t *rReg;
	volatile uint8_t *rOut;
	volatile uint8_t *rIn;
	unsigned long totalS;
	unsigned long totalR;
	
	unsigned int* sBaseline;
	unsigned int* rBaseline;
	
	unsigned int Avect;
	unsigned int Bvect;							  
	unsigned int Cvect;	
	unsigned int topVect, middleVect;

	float x, y;
	int angle;
	unsigned int Tvect;
	
	
		// calibrateFlag = 0;  // calibrate first time through the loop
	
	for (byte j=0; j<3; j++){
		
		totalR = 0;    
		totalS = 0;   
		
		// point accumulate register pointers to pins being sampled
		if (j==0){			
			// pins A & B
			
			sBit =	aBit;             // asssign send pointers to pin A
			sOut =	aOut;
			sIn =	aIn;
			sReg =	aReg;			
			
			rBit =	bBit;             // asssign receive pointers to pin B
			rOut =	bOut;
			rIn =	bIn;
			rReg =	bReg;
			
			if (calibrateFlag == 0){
				delay(1000); // delay 1 sec to give user time to get hands away from the sensor
				sBaseline = &Baseline[0];
				rBaseline = &Baseline[1];
			}
			else {
				sBaseline = &RawResult[0];
				rBaseline = &RawResult[1];
			}
		}
		else if (j==1){			
			// pins B & C
			
			sBit =	bBit;             // asssign send pointers to pin B
			sOut =	bOut;
			sIn =	bIn;
			sReg =	bReg;			
			
			rBit =	cBit;             // asssign receive pointers to pin C
			rOut =	cOut;
			rIn =	cIn;
			rReg =	cReg;
			
			if (calibrateFlag == 0){
				sBaseline = &Baseline[2];
				rBaseline = &Baseline[3];
			}
			else {
				sBaseline = &RawResult[2];
				rBaseline = &RawResult[3];
			}

		}
		else if (j==2){			
			// pins C & A
			
			sBit =	cBit;             // asssign send pointers to pin C
			sOut =	cOut;
			sIn =	cIn;
			sReg =	cReg;			
			
			rBit =	aBit;             // asssign receive pointers to pin A
			rOut =	aOut;
			rIn =	aIn;
			rReg =	aReg;	
			
			if (calibrateFlag == 0){
				sBaseline = &Baseline[4];
				rBaseline = &Baseline[5];
				calibrateFlag = 1;
			}
			else {
				sBaseline = &RawResult[4];
				rBaseline = &RawResult[5];
			}
		
		}
		
		for(int i =0; i < samples; i++){    // 1 extra to make up for calibration cycle
			
			// we will set up a send-receive cycle for each pin
			// in the second cycle, the role of the send and receive pins will be reversed
			// while the pin actions stay the same as the first pair
			
			
			*sReg |=  sBit;			// set send pin to OUTPUT
			*sOut &= ~sBit;			// set Send Pin Register LOW  to discharge send pin	
			
			*rOut &= ~rBit;			// receive pin LOW - because the bottom loop will exit when pin is ~ 2.5V 
			*rReg |= rBit;			// receive pin is LOW  AND OUTPUT to discharge pin
			delayMicroseconds(10);  // short pause to discharge pin
			
			*rReg &= ~rBit;			// set receive pin to INPUT 	
			*sOut |= sBit;			// set send pin High
			
			while( !(*rIn & rBit)  && (totalR < timeoutCount )){  // while receive pin is LOW && total < timeout value
				totalR++;
			}
			
			// set receive pin HIGH briefly to charge up fully - because the while loop above will exit when pin is ~ 2.5V 
			*rOut  |= rBit;			// set receive pin HIGH - turns on pullup 
			*rReg |= rBit;			// pin is now HIGH AND OUTPUT
			delayMicroseconds(10);  // short pause to discharge  pin
			
			*rReg &=  ~rBit;		// set pin to INPUT 
			*rOut  &= ~rBit;		// turn off pullup
			
			*sOut &= ~sBit;			// set send pin LOW
			
			while( (*rIn & rBit) && (totalR < timeoutCount)){   // while receive pin is HIGH  AND total is less than timeout
				totalR++;
			}
			
			
			// OK reverse the roles now so sIn is doing the receiving
			// receive pin is now doing the sending
			// actual pins in [brackets] in comments - maybe not worth the confusion
			
			*rReg |=  rBit;			// set send [receive] pin to OUTPUT
			*rOut &= ~rBit;			// set Send Pin Register LOW  to discharge send pin	
			
			*sOut &= ~sBit;			// receive [send] pin LOW - because the bottom loop will exit when pin is ~ 2.5V 
			*sReg |= sBit;			// receive [send] pin is LOW  AND OUTPUT to discharge pin
			delayMicroseconds(10);  // short pause to discharge pin
			
			*sReg &= ~sBit;			// set receive [send] pin to INPUT 	
			*rOut |= rBit;			// set send [receive] pin High
			
			while( !(*sIn & sBit)  && (totalS < timeoutCount )){  // while receive pin is LOW && total < timeout value
				totalS++;
			}
			
			// set receive pin HIGH briefly to charge up fully - because the while loop above will exit when pin is ~ 2.5V 
			*sOut  |= sBit;			// set receive [send] pin HIGH - turns on pullup 
			*sReg |= sBit;			// pin is now HIGH AND OUTPUT
			delayMicroseconds(10);  // short pause to discharge  pin
			
			*sReg &=  ~sBit;		// set receive [send] pin to INPUT 
			*sOut  &= ~sBit;		// turn off pullup
			
			*rOut &= ~rBit;			// set send [receive] pin LOW
			
			while( (*sIn & sBit) && (totalS < timeoutCount)){   // while receive pin is HIGH  AND total is less than timeout
				totalS++;
			}
		}
		
		// set pins back to inputs
		
		*sReg &=  ~sBit;		// set pin to INPUT 
		*sOut  &= ~sBit;		// turn off pullup
		*rReg &=  ~rBit;		// set pin to INPUT 
		*rOut  &= ~rBit;		// turn off pullup
		
		if (totalR >= timeoutCount || totalS >= timeoutCount){   // either variable over timeout - bail out
			Serial.println("Sensor read timed out, check your wiring.");
			wheelData.angle = -100;
			wheelData.touch = -100;
			return wheelData;
		}
		
		*sBaseline = totalS;
		*rBaseline = totalR;
		
	}

	for(int i = 0; i<6; i++){
		
		if (((long)RawResult[i] - (long)Baseline[i]) > -1)   RawResult[i] =  ( RawResult[i] - Baseline[i]);
		else RawResult[i] = 0;
		
		//	Serial.print(Baseline[i]);       
		//	Serial.print("\t");      
		//	Serial.print(RawResult[i]);		
		//  Serial.print("\t\t"); 
	}
	
		//  Serial.println("");		
	
	Avect = RawResult[0] + RawResult[5];
	Bvect = RawResult[1] + RawResult[2];							  
	Cvect = RawResult[3] + RawResult[4];
	
/*	
	Serial.print(" Avect = ");
	Serial.print(Avect);		
	Serial.print("\t\t"); 
	Serial.print(Bvect);		
	Serial.print("\t\t"); 
	Serial.print(Cvect);		
	Serial.print("\t\t"); 
	
	Serial.println("");	
	*/
	
	/* Add vectors - treat each vector as a force vector at
	 * 0 deg, 120 deg, 240 deg respectively
	 * parse out x and y components of each vector
	 * y = sin(angle) * Vect , x = cos(angle) * Vect
	 * add vectors then use atan2() to get angle
	 * vector quantity from pythagorean theorem
	 */

	x = (float)Avect -  (.5 * (float)Bvect ) - ( .5 * (float)Cvect); // integer math
	y = (.866 * (float)Bvect) - (.866 * (float)Cvect);

	angle = atan2((float)y, (float)x) * 57.296 + 180 ; // convert to degrees and lose the neg values
	Tvect = (long)sqrt( x * x + y * y);
	
	wheelData.angle = angle;
	wheelData.touch = Tvect;
	
	return wheelData;
	
} 
