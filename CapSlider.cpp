/*
 CapSlider.cpp v.05 - Capacitive Sensing Library for 'duino / Wiring
 Paul Bagder 2009-2012
 
 Implements a capacitive slider using some form of resistive bridge.
 */


// include this library's description file
#include "CapSlider.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#include "pins_arduino.h"
#include "WConstants.h"
#endif



// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

CapSlider::CapSlider(uint8_t sendPin, uint8_t receivePin){
	
	// initialize this instance's variables
	// Serial.begin(9600);       // for debugging
	//	error = 1;
    
	// Serial.print("timwOut =  ");
	
	
	// get pin mapping and port for send Pin - from PinMode function in Arduino core
	
	sBit =  digitalPinToBitMask(sendPin);           // get send pin's ports and bitmask
	sPort = digitalPinToPort(sendPin);
	sOut = portOutputRegister(sPort);               // get pointer to output register   
	sIn  = portInputRegister(sPort);				// get pointer to input register 
	sReg = portModeRegister(sPort);
	
	rBit = digitalPinToBitMask(receivePin);         // get receive pin's ports and bitmask 
	rPort = digitalPinToPort(receivePin);
	
   	rOut = portOutputRegister(rPort);
	
	if (sPort == NOT_A_PORT) {error = -1;           // this Arduino function does not appear to work
		// Serial.println("bad pin");
	}
	
	
	
	// get pin mapping and port for receive Pin - from digital pin functions in Wiring.c
	
	if (rPort == NOT_A_PORT){error = -1;           // this Arduino function does not appear to work
		// Serial.println("bad pin");
	}
	
	rReg = portModeRegister(rPort);
	rOut = portOutputRegister(rPort);
	rIn  = portInputRegister(rPort);
	
	*sReg |= sBit;              // set sendpin to OUTPUT 
	
	calibrateFlag = 0;
	
}

// Public Methods //////////////////////////////////////////////////////////////


// readSlider reads the sensor the first time through, and uses that value for
// baseline reading to calibrate the sensor. It takes a chip reboot to update the baseline values.

int CapSlider::readSlider(unsigned int samples){
	int i;
	
    totalR = 0;    // total for when "receive" pin is receiving (acting as the sensor pin)
	totalS = 0;    // total for when "send" pin is receiving
	
	
head:         // repeat for calibrate first time through
	
	for(i =0; i < samples; i++){
		
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
	
	
	if (calibrateFlag == 0){	
		rBaseline = totalR++;
		sBaseline = totalS++;
		calibrateFlag = 1;
		goto head;
	}
	
	/* Serial.print("rBaseline  ");
	 Serial.print(rBaseline);
	 Serial.print("  ");
	 Serial.print(totalR);
	 Serial.print("   ");
	 Serial.print(totalR - rBaseline);
	 
	 Serial.print("    sBaseline  ");
	 Serial.print(sBaseline);
	 Serial.print("  ");
	 Serial.print(totalS);
	 Serial.print("  ");
	 Serial.println(totalS - sBaseline); */
	
	if (totalR + totalS >= timeoutCount){
		Serial.println("Sensor over timeout. Check your wiring");
		return -2;         // total variable over timeout
	}
	
	totalR = totalR - rBaseline; 
	if (totalR < 0) totalR = 0;
    totalS = totalS - sBaseline;	
	if (totalS < 0) totalS = 0;
	
	/* Serial.print(totalR);
	Serial.print("  ");
	Serial.println(totalS); */
	
	pressure = totalR + totalS;
	
	if (pressure < SENSOR_NOT_TOUCHED){
		return 0;   // return zero instead of random noise for untouched sensor
	}
	else {
		return  ((float)totalR * 100.0) / (float)(totalR + totalS);  
	}
	
} 


