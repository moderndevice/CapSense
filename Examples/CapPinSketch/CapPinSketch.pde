#include <CapPin.h>

/* CapPin
 * Capacitive Library CapPin Demo Sketch
 * Paul Badger 2011
 * This class uses the bullt-in pullup resistors read the capacitance on a pin
 * The pin is set to input and then the pullup is set,
 * A loop times how long the pin takes to go HIGH.
 * The readPin method is fast and can be read 1000 times in under 10 mS.
 * By reading the pin repeated you can sense "hand pressure" 
 * at close range with a small sensor. A larger sensor (piece of foil/metal) will yield
 * larger return values and be able to sense at more distance. For 
 * a more senstive method of sensing pins see CapTouch
 * Hook up a wire with or without a piece of foil attached to the pin.
 * I suggest covering the sensor with mylar, packing tape, paper or other insulator
 * to avoid having users directly touch the pin. 
 */
 
CapPin cPin_5 = CapPin(5);   // read pin 5

float smoothed;

void setup()                    
{

  Serial.begin(115200);
  Serial.println("start");
 // slider_2_7.calibrateSlider();

}

void loop()                    
{


  delay(1);
  long total1 = 0;
  long start = millis();
  long total =  cPin_5.readPin(2000);


  // simple lowpass filter to take out some of the jitter
  // change parameter (0 is min, .99 is max) or eliminate to suit
  smoothed = smooth(total, .8, smoothed);   

Serial.print( millis() - start);      // time to execute in mS
Serial.print("\t");
Serial.print(total);                  // raw total
Serial.print("\t");
Serial.println((int) smoothed);       // smoothed
  delay(5);                       
}

// simple lowpass filter
// requires recycling the output in the "smoothedVal" param
int smooth(int data, float filterVal, float smoothedVal){

  if (filterVal > 1){      // check to make sure param's are within range
    filterVal = .999999;
  }
  else if (filterVal <= 0){
    filterVal = 0;
  }

  smoothedVal = (data * (1 - filterVal)) + (smoothedVal  *  filterVal);

  return (int)smoothedVal;
}






