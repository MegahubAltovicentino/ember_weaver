#include <Wire.h>

#include <MIDI.h>


MIDI_CREATE_DEFAULT_INSTANCE();


void setup()
{
    MIDI.begin();           // Launch MIDI, by default listening to channel 1.
    Serial.begin(31250);
    Wire.begin();       //master
    for(int i=0;i<=5;i++){
      Serial.print(".");
      delay(100);
    }
    Serial.println(".");
    Serial.println("Serial switch to 'ON'");
    
}

void loop()
{
  
    if (MIDI.read())                // Is there a MIDI message incoming ?
    {
      
        switch(MIDI.getType())      // Get the type of the message we caught
        {
            case midi::NoteOn:        
                Serial.println("noteOn");    
                Serial.println(MIDI.getData1());  //numero nota midi
                Serial.println(MIDI.getData2());  //velocity                                     
                Wire.beginTransmission(8); // transmit to device #8
                Wire.write("n"); //note
                Wire.write(MIDI.getData1());    
                Wire.endTransmission();    // stop transmitting

                Wire.beginTransmission(8); // transmit to device #8
                Wire.write("v");  //velocity
                Wire.write(MIDI.getData2());    
                Wire.endTransmission();    // stop transmitting
                break;
            
            case midi::NoteOff:
                Serial.println("noteOff");
                
                break;
        }
    }
    
}
