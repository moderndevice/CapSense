/*
 CapacitiveSense.h v.03 - Capacitive Sensing Library for 'duino / Wiring
 Copyright (c) 2008 Paul Bagder  All rights reserved.
 */

// ensure this library description is only included once
#ifndef CapSlider_h
#define CapSlider_h


#define US  67UL  // period of four US powerline cycles in uS   ( 1/60 * 4 ) * 1000000
#define EU  80UL  // period of four EU powerline cycles in uS   ( 1/50 * 4 ) * 1000000

#define calibrateTime US  // change to EU for 50 cycle power countries

// count after which functions timeout - arbitrary units
// ground one pin through a 1k resistor to see how long this takes

#define timeoutCount 4000000L 

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


// library interface description
class CapSlider
{
	// user-accessible "public" interface
public:
	
	// variables
    unsigned long  total;
	int error;
	unsigned int sPerIterationBaseline;  // send pin baseline calibration measurement
	unsigned int rPerIterationBaseline;  // receive pin baseline calibration measurement
	
	unsigned long totalS;
	unsigned long totalR;

	
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
	
	// calibrate the baseline value for Slider method
	void calibrateSlider();
	
	// two pins with two directional sensing for sliders
	long readSlider(unsigned int samples);
};

#endif