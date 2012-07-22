/*
 CapPin.cpp - Capacitive Sensing Library for 'duino / Wiring
 8-8-2011 <paul@moderndevice.com> http://opensource.org/licenses/mit-license.php
 This class uses the Atmega's built-in pullup resistors in place of the "send pin" in 
 CapSlider, and CapTouch.
 */

// include this library's description file
#include "CapPin.h"


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "pins_arduino.h"
#include "WConstants.h"
#endif






// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

CapPin::CapPin(uint8_t sensorPin){
	
	// initialize this instance's variables
	// Serial.begin(9600);       // for debugging
	int error = 1;
	
	// no "send Pin" in this method - the Port register pullup does all the work
	// get pin mapping and port for sensorPin - from digital pin functions in Wiring.c
	
	rBit = digitalPinToBitMask(sensorPin);    // get receive pin's ports and bitmask 
	rPort = digitalPinToPort(sensorPin);
	
   	rOut = portOutputRegister(rPort);
	
	if (rPort == NOT_A_PORT){ error = -1;     // this Arduino function does not appear to work
		// Serial.println("bad pin");
	}
	
	rReg = portModeRegister(rPort);
	rOut = portOutputRegister(rPort);
	rIn  = portInputRegister(rPort);
		
	calibrateFlag = 0;
	
}

// calibrate the baseline value for readPin method
void CapPin::calibratePin(unsigned int samples){
	
	uint16_t  j, k=0; 
	total = 0;
	
	*rReg |= rBit;             // set Send pin Output
	
	// the idea here is to calibrate for the same number of samples that are specified
	// but to make sure that the value is over a certain number of powerline cycles to 
	// average out powerline errors
	
	unsigned long start = millis();
	*rOut  &= ~rBit;       // set sensorPin output register LOW 
	
	while (millis() - start < calibrateTime){   // sample at least 10 power line cycles
		
		for (unsigned int i = 0; i < samples; i++){   // loop for samples parameter 
			
			*rReg &= ~rBit;        // set sensorPin to INPUT
			*rOut |= rBit;         // set sensorPin output register HIGH to set pullups
			
			
			for( j = 0; j < 500; j++)
			{  if ( *rIn & rBit ) 
				break; }
			
			total+= j;
			
			*rOut  &= ~rBit;       // Pin output register LOW 
			*rReg |= rBit;         // OUTPUT & LOW now	
		}
		k++;		
	}
	
	// if pin is grounded (or connected with resistance lower than the pullup resistors, 
	// the method will return in about one second.
	if (total >= ((unsigned long)samples * 450UL * (unsigned long)k)){
		Serial.println("calibratePin method over timeout, check wiring.");
	}
    else
    {   
		baselineCount = total / k;
		Serial.print("Calibrated baselineCount = ");
		Serial.println(baselineCount);
	} 
}					 


// uses the pullups to sense capacitance without a send pin

long CapPin::readPin(unsigned int samples)
{   uint16_t j;
	total = 0;
	
	*rOut |= rBit; 
	
	//	if (samples < 1) return 0;	  // defensive programming
	
	// calibrate first time through after reset or cycling power
	if (calibrateFlag == 0 || samples != lastSamples){
		calibratePin(samples);
		lastSamples = samples;
		calibrateFlag = 1;		
	}
	
	
	// I'm using a for loop here instead of while. It adds a timeout automatically
	// the idea is from here http://www.arduino.cc/playground/Code/CapacitiveSensor  
	for (unsigned int i = 0; i < samples; i++){   // loop for samples parameter 
		
		noInterrupts();
		*rReg &= ~rBit;        // set sensorPin to INPUT
		*rOut |= rBit;         // set sensorPin output register HIGH to set pullups
		interrupts();
		
		for( j = 0; j < 5000; j++)  
		{  if ( *rIn & rBit )  // bail out when pin goes high
			break; }
		
		total+= j;
		
		noInterrupts();
		*rOut  &= ~rBit;       // Pin output register LOW 
		*rReg |= rBit;         // OUTPUT & LOW now
		interrupts();
	}
	
	// if pin is grounded (or connected to ground with resistance 
	// lower than the pullup resistors, 
	// the method will return in about one second.
	if (total >= ((unsigned long)samples * 450UL)) {
		Serial.println("readPin method over timeout, check wiring.");
		return -1;
	}
	
	
	//Serial.println(total);
	
#ifdef NO_NEGATIVES
	

	if (((long)total - (long)baselineCount) > 0) 
		return (total - baselineCount); 
	else return 0;  
#else
	return (total - baselineCount);  
#endif  
	
}


