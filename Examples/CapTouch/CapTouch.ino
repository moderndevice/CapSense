#include <CapTouch.h>

/* CapTouch - an example for the CapTouch class
* from the v.04 - Capacitive Sensing Library for 'duino / Wiring
* by Paul Badger 2-3-2012
* <paul@moderndevice.com> http://opensource.org/licenses/mit-license.php
*
* This capacitive sensing method requires two microcontroller pins (send pin, receive pin)
* with a high value resistor between them (1M is a starting point)
* a small capacitor (20-40 pf) from the receive pin (the sensor is connected to this pin)
* to ground will result in more stable readings. Sensor is any wire or conductive foil 
* on the receive pin.
*
* Also demonstrates a simple smoothing filter which is not required in any way
* by the sketch 
*/

 
// CapTouch(sendPin, receivePin) - recieve pin is the sensor to touch
 CapTouch touch_2_5 = CapTouch(2,5); 

float smoothed;

void setup()                    
{

  Serial.begin(9600);
  Serial.println("start");

}

void loop()                    
{

  long start = millis();                    // scheme to time performance, delete at will
  long total =  touch_2_5.readTouch(15);    //  read the sensor
  long elapsedTime =  millis() - start;     // scheme to time performance, delete at will

  // simple lowpass filter to take out some of the jitter
  // change parameter (0 is min, .99 is max) or eliminate to suit
  // you need a different "smoothed" variable for each sensor is using more than one
  smoothed = smooth(total, .95, smoothed);   

  Serial.print(elapsedTime);
  Serial.print(" ms      ");
  Serial.print(total);
  Serial.print("     ");
  Serial.println(smoothed);       // print sensor output 1

  delay(5);                       
}



// simple lowpass filter
// requires recycling the output in the "smoothedVal" param
float smooth(float data, float filterVal, float smoothedVal){

  if (filterVal > 1){      // check to make sure param's are within range
    filterVal = .99;
  }
  else if (filterVal <= 0){
    filterVal = 0;
  }

  smoothedVal = (data * (1 - filterVal)) + (smoothedVal  *  filterVal);

  return smoothedVal;
}






