#include <Wire.h>
#include <MIDI.h>

#define I2C_DEVICE 9
#define DEBUG_RCV false
#define DEBUG_SND false

MIDI_CREATE_DEFAULT_INSTANCE();

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  Serial.begin(31250); 
  Wire.begin();
  for(int i = 0; i <= 5; i++) {
    Serial.print(".");
    delay(100);
  }
  Serial.println("Listening for MIDI.");
}

void loop() {
  if (MIDI.read()) {
    byte type = MIDI.getType();
    byte channel = MIDI.getChannel();
    byte data1 = MIDI.getData1();
    byte data2 = MIDI.getData2();
    if (DEBUG_RCV) {
      Serial.print("MIDI Data: ");
      Serial.print("0x");
      Serial.print(type, HEX);
      Serial.print(" 0x");
      Serial.print(data1, HEX);
      Serial.print(" 0x");
      Serial.println(data2, HEX);
      Serial.print("Channel: ");
      Serial.println((int) channel);
    }

    if (type == 0x90) { // noteOn
      if (DEBUG_RCV) {   
        Serial.print("noteOn ");    
        Serial.print((int) data1);
        Serial.print(" ");
        Serial.println((int) data2);
      }
      sendMessage("n", data1, data2);
    } else if (type == 0x80) { // noteOff
      if (DEBUG_RCV) {
        Serial.print("Note Off: ");
        Serial.print((int) data1);
        Serial.print(" ");
        Serial.println((int) data2);
      }
    } else if (type == 0xB0) {
      int controlNumber = (int) data1;
      int controlValue = (int) data2;
      if (DEBUG_RCV) {
        if (controlNumber == 1) {
          if (channel == 1) {
            Serial.print("Modulation Wheel: ");
          } else if (channel == 2) {
            Serial.print("Pitch bend: ");
          }
          Serial.println(controlValue);
        } else {
          Serial.print("Control number ");
          Serial.print(controlNumber);
          Serial.print(": ");
          Serial.println(controlValue);
        }
      }
      sendMessage("c", data1, data2);
    } else {
      Serial.print("Unknown MIDI Message Type: ");
      Serial.println(type, HEX); 
    }
  }
}

void sendMessage(const char* msg, byte data1, byte data2) {
  if (DEBUG_SND) {
    Serial.print("SENDING: ");
    Serial.print(msg);
    Serial.print(" ");
    Serial.print(data1);
    Serial.print(" ");
    Serial.print(data2);
    Serial.println("...");
  }

  digitalWrite(LED_BUILTIN, HIGH);
  Wire.beginTransmission(I2C_DEVICE);
  Wire.write(msg);
  Wire.write(data1);    
  Wire.write(data2);    
  Wire.endTransmission();
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);

  if (DEBUG_SND) {
    Serial.println(" SENT"); 
  }
}
