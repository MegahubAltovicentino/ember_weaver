#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#include <Wire.h>

#define PIN        7
#define NUMPIXELS 34
#define DEVICE 9
#define DEBUG true

int r = 150;
int g = 150;
int b = 150;  

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); 
  Wire.begin(DEVICE);
  // Wire.setClock(400000);
  Serial.begin(9600);           
  pixels.begin();
  Wire.onReceive(receiveMessage); 
  Serial.println("Listening...");
}

void loop() {  
  delay(100);
}

void receiveMessage(int bytes) {  
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);

  if (bytes >= 3) {
    char msg = Wire.read();  // Read the message (first byte)
    byte data1 = Wire.read();  // Read data1 (second byte)
    byte data2 = Wire.read();  // Read data2 (third byte)

    if (DEBUG) {
      Serial.print("Received Message: ");
      Serial.print("msg: ");
      Serial.print(msg);
      Serial.print(" data1: ");
      Serial.print(data1);
      Serial.print(" data2: ");
      Serial.println(data2);
    }

    if (msg == 'n') {
      Serial.print("Received note message: ");
      Serial.print(data1);
      Serial.print(" ");
      Serial.println(data2);
    } 
    else if (msg == 'c') {
      Serial.println("Received control message");
      Serial.print(data1);
      Serial.print(" ");
      Serial.println(data2);
    }
  } else {
    Serial.println("Error: Insufficient bytes received.");
  }  
}
