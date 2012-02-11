/*  CapScroll.pde  rev01
 * Example sketch for Capacitive Scroll Wheel Library
 * Paul Bagder 2009-2011
 * 9-27-2011 <paul@moderndevice.com> http://opensource.org/licenses/mit-license.php 
 * For use with a resistive scroll wheel with three taps around the circle
 * to implement a scroll wheel. A description of a functional scroll wheel
 * follows. Divide a self adhesive copper foil "wheel" shape (concentric circles) with
 * a narrowish hub into 24 to 30 equall truncated pie slice shapes(modulo three). 
 * Cut slots at the pie-slice borders,
 * just wide enough to accomodate a surface mount resistor. 
 * (use whatever form factor resistors you feel comfotable with - 
 * 1206 size might be good for beginner sm assemblers).
 * solder a 100K sm resistor between every pie slice. Solder three wires in equal
 * angles around the circle. After testing for resistive continuity - cover the whole
 * thing in clear packing tape or equivalent. Plug the three wires into three free pins
 * on a Freeduino (this includes all Arduinos) or other microcontroller.
 * Small caps (~20pf)  between sensor wires and ground at the board
 *  will improve stability and jitter.
 */

#include <CapScroll.h>

// scroll wheel wires A,B,C on pins 3,6,9
// pins are not important, works on any pins
CapScroll Scroll3_6_9 = CapScroll(7,9,11); 

// datatype defined in the libary - hence double colons
ScrollWheelData scrollData;

float smoothed;

void setup()                    
{
  // serial speed is not important
  Serial.begin(115200);                                 // different baud rate is fine 
  Serial.println("start");
}

void loop()                    
{
  unsigned long start = millis();

  scrollData =  Scroll3_6_9.readScroll(10);             // read 10 samples, experiment with number!
  smoothed = smooth(scrollData.angle, .6, smoothed);
  
  Serial.print(millis() - start);                       // execution time in mS -  optional
  Serial.print("\t\t");
  Serial.print(scrollData.angle);
  Serial.print("\t\t");
  Serial.print((int)smoothed);                          // low pass filtered - optional
  Serial.print("\t\t");
  Serial.println(scrollData.touch);

  delay(100);                       
}

 // simple lowpass filter to take out some of the jitter
 // change parameter (0 is min, .99 is max) or eliminate to suit
 // smoothed = smooth(total, .9, smoothed);  
 // requires recycling the output in the "smoothedVal" param
 // this smoothing filter will have issues at the "origin" where 359 degrees rolls over to 0
 // I have a good solution for this (really) but I'll leave it for another time.
int smooth(int data, float filterVal, float smoothedVal){

  if (filterVal > 1){      // check to make sure param's are within range
    filterVal = .99;
  }
  else if (filterVal <= 0){
    filterVal = 0;
  }

  smoothedVal = (data * (1 - filterVal)) + (smoothedVal  *  filterVal);

  return (int)smoothedVal;
}









