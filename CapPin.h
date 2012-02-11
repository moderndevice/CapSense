/*
 CapPin.h - Capacitive Sensing Library for 'duino / Wiring
 8-8-2011 <paul@moderndevice.com> http://opensource.org/licenses/mit-license.php
 This class uses the Atmega's built-in pullup resistors in place of the "send pin" in 
 CapSlider, and CapTouch.
 */

// ensure this library description is only included once
#ifndef CapPin_h
#define CapPin_h

#define US  166L  // period of ten US powerline cycles in uS   ( 1/60 * 10 ) * 1000
#define EU  200L  // period of ten EU powerline cycles in uS   ( 1/50 * 10 ) * 1000

#define calibrateTime US  // change to EU for 50 cycle power countries

// count after which functions timeout - arbitrary units
// ground one pin through a 1k resistor to see how long this takes

#define NO_NEGATIVES  // don't return values less than 0 - comment out to see negative values

// include types & constants of Wiring core API

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
// include core Wiring API
#include "WProgram.h"
#endif



// library interface description
class CapPin
{
  // user-accessible "public" interface
  public:
  
  // variables
    unsigned long  total;
	unsigned long baselineCount;
	
	uint8_t rBit;    // receive pin's ports and bitmask 
	uint8_t rPort;
	volatile uint8_t *rReg;
	volatile uint8_t *rIn;
	volatile uint8_t *rOut;
	uint8_t calibrateFlag;
	unsigned int lastSamples;
  
    // methods
	
	// constructor
    CapPin(uint8_t sensorPin);
		
	// calibrate the baseline value for readPin method
	void calibratePin(unsigned int samples);
	
	// readPin method uses cacitive sensing with the built-in pullup resistor
	long readPin(unsigned int samples);
	
};

#endif