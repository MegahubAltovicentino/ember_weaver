#include <OSCMessage.h>
#include <SLIPEncodedSerial.h>  // To decode OSC over serial

SLIPEncodedSerial SLIPSerial(Serial);

void setup() {
  Serial.begin(9600);  // Match the baud rate with Pure Data's comport object
}

void parseMIDI(OSCMessage &msg) {
  if (msg.fullMatch("/midi/note")) {
    int note = msg.getInt(0);
    int velocity = msg.getInt(1);

    // Print to verify reception
    Serial.print("Note: ");
    Serial.print(note);
    Serial.print(", Velocity: ");
    Serial.println(velocity);

    // Here you could send MIDI messages using MIDI.h or other logic
  }
}

void loop() {
  while (SLIPSerial.available()) {
    OSCMessage msg;
    msg.empty();
    msg.fill(SLIPSerial.read());

    if (!msg.hasError()) {
      parseMIDI(msg);
    }
  }
}