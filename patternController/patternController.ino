// --------------------------------------------------------------------------------------------------
// patternController.ino
// 
// The pattern controller enables the user to select single-frame patterns, namely:
// - flash
// - snake
// - sparkle
// - rainfall
// - pulse / fade
// 
// User is also able to control the following:
// - color
// - color change
// - frequency (e.g. pattern flashing)
// 
// Reference
// Red: strip.Color(255, 0, 0)
// Green: strip.Color(0, 255, 0)
// Blue: strip.Color(0, 0, 255)
// White: strip.Color(127, 127, 127)
// --------------------------------------------------------------------------------------------------

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#include <Wire.h>

#define PIXEL_PIN 7 
#define PIXEL_COUNT 80  
#define I2C_CHANNEL 9
#define DEBUG true

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
int mode = 0;    // Currently-active animation mode, 0-9
uint32_t color = strip.Color(255, 0, 0);

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Wire.begin(I2C_CHANNEL);
  Wire.onReceive(receiveMessage); 
}

void receiveMessage(int bytes) {  
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

void loop() {

  while(!Serial.available()){}
  int program_selection = Serial.parseInt();

  switch(program_selection) {
    case 0:
      Serial.println("off");
      setColor(strip.Color(0, 0, 0));
      break;
    case 1: 
      Serial.println("pick color...");
      color = pickColor();
      Serial.println("...done");
      break;
    case 2:
      Serial.println("set");
      setColor(color);
      break;
    case 3:
      Serial.println("flash");
      flash(color, 50);
      break;
    case 4:
      Serial.println("snake");
      snake(color, -1, 50);
      break;
    case 5:
      Serial.println("sparkle");
      break;
    case 6:
      Serial.println("rainfall");
      break;
    case 7:
      Serial.println("pulse");
      pulse(color, 1000);
      delay(50);
      pulse(color, 1000);
      delay(50);
      pulse(color, 1000);
      break;
    case 8:
      Serial.println("theater chase");
      theaterChase(color, 50); // White
      break;
    case 9:
      Serial.println("rainbow");
      rainbow(10);
      break;
    case 10:
      Serial.println("rainbow theater chase");
      theaterChaseRainbow(50);
      break;
  }
}

// Fill strip pixels one after another with a color. 
void setColor(uint32_t color) {
  for (int i = 0; i < strip.numPixels(); i++) { 
    strip.setPixelColor(i, color);                                                 
  }
  strip.show(); 
}

void flash(uint32_t color, int wait) {
  strip.clear(); 
  strip.show();
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
  delay(wait);
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
}

// negative dir indicates clockwise; positive dir is counterclockwise
void snake(uint32_t color, int dir, int wait) {
  strip.clear(); 
  strip.show();
  if (dir > 0) {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i * dir, color);
      strip.show();
      delay(wait);
    }
  } else if (dir < 0) {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(strip.numPixels() - i - 1, color);
      strip.show();
      delay(wait);
    }
  }
}

void pulse(uint32_t color, int ms) {
  strip.clear(); 
  strip.show();

  uint8_t red = (color >> 16) & 0xFF;
  uint8_t green = (color >> 8) & 0xFF; 
  uint8_t blue = color & 0xFF;
  
  int num_steps = 100;
  int step = ms / 2 / num_steps;

  for (int i = 0; i < num_steps; i++) {
    float brightness = (float) i / num_steps;
    uint8_t r = red * brightness;
    uint8_t g = green * brightness;
    uint8_t b = blue * brightness;
    for (int pixel = 0; pixel < strip.numPixels(); pixel++) {
      strip.setPixelColor(pixel, strip.Color(r, g, b));
    }
    strip.show();
    delay(step);
  }
  for (int i = num_steps; i >= 0; i--) {
    float brightness = (float) i / num_steps;
    uint8_t r = red * brightness;
    uint8_t g = green * brightness;
    uint8_t b = blue * brightness;
    for (int pixel = 0; pixel < strip.numPixels(); pixel++) {
      strip.setPixelColor(pixel, strip.Color(r, g, b));
    }
    strip.show();
    delay(step);
  }
}

// Theater-marquee-style chasing lights.
void theaterChase(uint32_t color, int wait) {
  for(int a = 0; a < 10; a++) {  
    for(int b = 0; b < 3; b++) { 
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c = b; c < strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color);
      }
      strip.show(); 
      delay(500);  
    }
  }
}

// Rainbow cycle along whole strip. 
void rainbow(int wait) {
  // Hue of first pixel runs 3 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 3*65536. Adding 256 to firstPixelHue each time
  // means we'll make 3*65536/256 = 768 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 3*65536; firstPixelHue += 256) {
    for(int i = 0; i < strip.numPixels(); i++) { 
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); 
    delay(wait);  
  }
}

// Rainbow-enhanced theater marquee. 
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a = 0; a < 30; a++) {  
    for(int b = 0; b < 3; b++) { 
      strip.clear();         
      for(int c = b; c < strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); 
        strip.setPixelColor(c, color); 
      }
      strip.show();                
      delay(wait);                 
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}

uint32_t pickColor() {
  Serial.println("Insert color 1");
  while(!Serial.available()){}
  int color_1 = Serial.parseInt();
  if (color_1 <= 255 && color_1 >= 0) {
    Serial.println("Insert color 2");
    while(!Serial.available()){}
    int color_2 = Serial.parseInt();
    if (color_2 <= 255 && color_2 >= 0) {
      Serial.println("Insert color 3");
      while(!Serial.available()){}
      int color_3 = Serial.parseInt();
      if (color_3 <= 255 && color_3 >= 0) {
        return strip.Color(color_1, color_2, color_3);
      } 
    }
  }
  Serial.println("invalid input");
}
