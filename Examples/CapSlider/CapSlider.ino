
/*  CapSlider.ino
 *  Paul Badger 6-2012
 *  Demo sketch for the CapSlider class
 *  Requires a resistive bridge of some form, between two wires
 *  The resistive bridge is read capacitively
 *  Adding some small (20-40 pf) caps at the pins helps stability and jitter.
 */
 
 #include <CapSlider.h>

// requires a resistive slider setup between send and receive pins - see the docs
// use any pins you like
CapSlider slider_11_12 = CapSlider(11,12); 

int potVal;

void setup()                    
{
    Serial.begin(9600);
    Serial.println("start");
}

void loop()                    
{   // The readSlider() method is designed to return values in between about 15 and 85 depending on how the slider is wired.
    // You can either live with these values or map them to a larger range (as below).
    // Because the library calibrates the first time through the readSlider() function - keep your hands off 
    // the snesor while the chip boots.
    // Once the the readSlider() method is called the first time, there is no further calibration.
    // A chip reset will be required to re-calibrate the slider.
    
    // A capacitive slider will not work just like a slide pot since it is necessary to see if a user has actually touched 
    // the slider, before changing parameters that have been hooked up to the slider.
    // You can query the class variable "total" to see what the "touched" value is and make decisions based upon that.
    // The "smooth" function in the Arduino Playground might be useful for taking out jitter.
    
    long start = millis();
    
    int total = map(slider_11_12.readSlider(30),14,81,0,100);  // adjust map input to returned values for ends of slider
    total = constrain(total, 0, 100);                          // constrain endpoints 

    Serial.print(millis() - start);             // check on performance in milliseconds
    Serial.print("\t");                         // tab character for debug windown spacing
    Serial.print(total);                        // print sensor output 1
    Serial.print("\t");                         // tab character for debug windown spacing
    Serial.print(slider_11_12.pressure);
    
    if (slider_11_12.pressure > 550){           // adjust per your slider / also per sample param in readSlider()
        potVal = total;
    }
 
    Serial.print("\t pot val = "); 
    Serial.println(potVal);

    delay(50);                                  // arbitrary delay to limit data to serial por    
}

