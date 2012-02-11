/*
 * CapScroll.h  rev01 - Capacitive Scroll Wheel Library for 'duino / Wiring
 * Paul Bagder 2009-2011
 *  9-27-2011 <paul@moderndevice.com> http://opensource.org/licenses/mit-license.php
 
 * Uses a resistive scroll wheel with three taps around the circle
 * to implement a scroll wheel. A description of a functional scroll wheel
 * follows. Divide a self adhesive copper foil "wheel" shape (concentric circles)
 * into 24 to 30 equall truncated pie slice shapes(modulo three). Cut slots at the slice borders,
 * just wide enough to accomodate a surface mount resistor. 
 * (use whatever form factor resistors you feel comfotable with - 
 * 1206 size might be good for beginner sm assemblers).
 * solder a 100K sm resistor between every pie slice. Solder three wires on pie slices equal
 * angles around the circle. After testing for resistive continuity - cover the whole
 * thing in clear packing tape or equivalent. Plug the three wires into three free pins
 * on a Freeduino (this includes all Arduinos) or other appropriate microcontroller.
 */


struct ScrollWheelData{
	int angle;
	unsigned int touch;  // touch pressure value
};


// ensure this library description is only included once
#ifndef CapScroll_h
#define CapScroll_h

// count after which functions timeout - arbitrary units
#define timeoutCount 400000L 

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#include "pins_arduino.h"
#include "WConstants.h"
#endif


//wheelData myData;

// library interface description
class CapScroll
{
	// user-accessible "public" interface
public:
	
	// variables
	unsigned int Baseline[6];  // Baseline measurements for three pairs
	unsigned int RawResult[6];  // Results measurements for three pairs
	char calibrateFlag;
	
	unsigned long totalA;
	unsigned long totalB;
	unsigned long totalC;
	
	uint8_t aBit;   // pinA's ports and bitmask
	uint8_t aPort;
	volatile uint8_t *aReg;
	volatile uint8_t *aOut;
	volatile uint8_t *aIn;
	
	uint8_t bBit;   // pinB's ports and bitmask
	uint8_t bPort;
	
	volatile uint8_t *bReg;
	volatile uint8_t *bOut;
	volatile uint8_t *bIn;
	
	uint8_t cBit;   // pinC's ports and bitmask
	uint8_t cPort;
	volatile uint8_t *cReg;
	volatile uint8_t *cOut;
	volatile uint8_t *cIn;
	ScrollWheelData wheelData;
	
    // methods
	// constructor
    CapScroll(uint8_t pinA, uint8_t pinB, uint8_t pinC);
	
	ScrollWheelData readScroll(unsigned int samples);
	
};

#endif