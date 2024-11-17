#include <MIDI.h>

// Create MIDI instance using default Serial port (USB on most Arduinos)
MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  Serial.begin(31250);  // Set baud rate to communicate with Arduino via USB (for debugging)
  MIDI.begin(MIDI_CHANNEL_OMNI);  // Start MIDI communication on all channels
  Serial.println("MIDI Receiver Started");
}

void loop() {
  MIDI.read();  // Read incoming MIDI messages
}

void handleNoteOn(byte channel, byte note, byte velocity) {
  Serial.print("Note On - ");
  Serial.print("Channel: ");
  Serial.print(channel);
  Serial.print(", Note: ");
  Serial.print(note);
  Serial.print(", Velocity: ");
  Serial.println(velocity);
}

void handleNoteOff(byte channel, byte note, byte velocity) {
  Serial.print("Note Off - ");
  Serial.print("Channel: ");
  Serial.print(channel);
  Serial.print(", Note: ");
  Serial.print(note);
  Serial.print(", Velocity: ");
  Serial.println(velocity);
}