/* CapTouch.h part of v.04 - Capacitive Sensing Library for 'duino / Wiring
 * by Paul Badger 2-3-2012
 * <paul@moderndevice.com> http://opensource.org/licenses/mit-license.php
 *
 * This capacitive sensing method requires two microcontroller pins (send pin, receive pin)
 * with a high value resistor between them (1M is a starting point)
 * a small capacitor (20-40 pf) from the receive pin (the sensor is connected to this pin)
 * to ground will result in more stable readings. Sensor is any wire or conductive foil 
 * on the receive pin.
 */

// ensure this library description is only included once
#ifndef CapTouch_h
#define CapTouch_h

#define US  166UL  // period of four US powerline cycles in uS   ( 1/60 * 10 ) * 1000
#define EU  200UL  // period of four EU powerline cycles in uS   ( 1/50 * 10 ) * 1000

#define calibrateTime US  // change to EU for 50 cycle power countries

//#define NO_NEGATIVES // prevent negative numbers for output

// count after which functions timeout - arbitrary units
// ground one pin through a 1k resistor to see how long this takes

#define timeoutCount 40000000L 

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// library interface description
class CapTouch
{
  // user-accessible "public" interface
  public:
  
  // variables
	int error;
	long  total;
	unsigned long baselineR;
	uint8_t calibrateFlag;
	unsigned int lastSamples;

	uint8_t sBit;   // send pin's ports and bitmask
	uint8_t sPort;
	volatile uint8_t *sReg;
	volatile uint8_t *sOut;
	
	uint8_t rBit;    // receive pin's ports and bitmask 
	uint8_t rPort;
	volatile uint8_t *rReg;
	volatile uint8_t *rIn;
	volatile uint8_t *rOut;
	
    // methods
	
	// constructor
    CapTouch(uint8_t sendPin, uint8_t receivePin);
	
	long readTouch(uint8_t samples);
		
	// calibrate the baseline value for Slider method
	long calibrateTouch(uint8_t samples);

};

#endif