
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#include <Wire.h>


#define PIN        7
#define NUMPIXELS 80


int r = 150;
int g = 150;
int b = 150;  

char c="";
char c1="";

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Wire.begin(8);                // join I2C bus with address #8
  Wire.setClock(400000);
  Serial.begin(9600);           // start serial for output
  pixels.begin();

  Wire.onReceive(receiveEvent); // register event
}

void loop() {
  
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {  

  
  while (1 < Wire.available()) { // loop through all but the last
    c = Wire.read(); // receive byte as a character
    //Serial.print("c: ");
    //Serial.println(c);         // print the character
    if (c=='c'){
      c1 = Wire.read();
      //Serial.print("c1: ");
      //Serial.println(c1);
    }
    
    
  }  
  int x = Wire.read();    // receive byte as an integer
  //Serial.print("x: ");
  //Serial.println(x);
  if(c=='c'){ // control change (cambio colore)
    switch (c1){
      case '!': // red
      r=2*x;
      //Serial.print("red changed to: ");
      //Serial.println(r);
      break;

      case '"': // green
      g=2*x;
      //Serial.print("green changed to: ");
      //Serial.println(g);
      break;

      case '#': // blue
      b=2*x;
      //Serial.print("blue changed to: ");
      //Serial.println(b);
      break;
    }  
  }      

  if(c=='n'){ // noteON

    if(x==101){ // caso fill
      pixels.fill(pixels.Color(r, g, b));
      pixels.show(); 
    }
    else{
      pixels.setPixelColor(x, pixels.Color(r, g, b));
      pixels.show();  
    }
      
  }

  if(c=='x'){ // noteOFF
    if(x==101){
      pixels.fill(pixels.Color(0, 0, 0));
      pixels.show(); 
    }
    else{
      pixels.setPixelColor(x, pixels.Color(0, 0, 0));
      pixels.show();  
    }    
  }
  
}
