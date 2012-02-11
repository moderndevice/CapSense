#include <CapSlider.h>

/*
 * CapitiveSense Library Demo Sketch
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10M between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 */

// requires a resistive slider setup between send and receive pins - see the docs
CapSlider slider_2_7 = CapSlider(2,7); 

void setup()                    
{

  
     Serial.begin(9600);
  Serial.println("start");
  
  slider_2_7.calibrateSlider();
  


}

void loop()                    
{


  delay(1);
    long total1 = 0;
    long start = millis();
    
int total = map(slider_2_7.readSlider(30),22,80,0,100);
     
     
   // while(millis() < start + 5){
 //    total1 +=  slider_2_7.calibrateSlider();
//    long total2 =  cs_4_5.cSenseTouch(10);
//    long total3 =  cs_4_8.cSenseTouch(10);
 //   }
 
 //  Serial.println(millis() - start);        // check on performance in milliseconds
    Serial.print("      ");                    // tab character for debug windown spacing

   Serial.println(total);                  // print sensor output 1
//    Serial.print("\t");
//    Serial.print(total2);                  // print sensor output 2
//    Serial.print("\t");
//    Serial.println(total3);                // print sensor output 3

    delay(5);                             // arbitrary delay to limit data to serial port 
}
