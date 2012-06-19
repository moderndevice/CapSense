/*
 CapSlider.h v.05 - Capacitive Sensing Library for 'duino / Wiring
 Paul Bagder 2009-2012
 
 Implements a capacitive slider using some form of resistive bridge.
 */

// ensure this library description is only included once
#ifndef CapSlider_h
#define CapSlider_h

// count after which functions timeout - arbitrary units
// ground one pin through a 1k resistor to see how long this takes

#define timeoutCount 4000000L 

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define SENSOR_NOT_TOUCHED 25     // value below which read will return zero (0)


// library interface description
class CapSlider
{
	// user-accessible "public" interface
public:
	
	// variables
     long  pressure;
	int error;
	 long sBaseline;  // send pin baseline calibration measurement
	 long rBaseline;  // receive pin baseline calibration measurement
	
	 long totalS;
	 long totalR;
	uint8_t calibrateFlag;

	
	uint8_t sBit;   // send pin's ports and bitmask
	uint8_t sPort;
	volatile uint8_t *sReg;
	volatile uint8_t *sOut;
	volatile uint8_t *sIn;
	
	uint8_t rBit;    // receive pin's ports and bitmask 
	uint8_t rPort;
	volatile uint8_t *rReg;
	volatile uint8_t *rIn;
	volatile uint8_t *rOut;
	
    // methods
	
	// constructor
    CapSlider(uint8_t sendPin, uint8_t receivePin);
		
	// two pins with two directional sensing for sliders
	int readSlider(unsigned int samples);
};

#endif