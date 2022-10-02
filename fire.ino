#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN    8
#define LED_COUNT  25

//This sets the max brightness for the entire strip, and is also used in determining an individual pixel's brightness.
//Max brightness 0-255
#define BRIGHTNESS 255

struct color
{
  byte R;
  byte G;
  byte B;
};

struct pixel
{
  int address;
  color pixelColor;
  int lifetime;
};

const color barn_red = {128, 17, 0};
const color mystic_red = {244, 75, 0};
const color engineer_orange = {182, 34, 3};
const color sinopia = {215, 53, 2};
const color orange = {252, 100, 0};
const color gold = {250, 192, 0};
const color yellow = {255, 230, 0};
const color yellow_orange = {255, 206, 0};
const color sunny = {245, 240, 108};
const color mango_tango {255, 138, 72};


color colors[] = {yellow_orange, mystic_red, engineer_orange, sinopia, orange, barn_red, gold, yellow};
int numColors = sizeof(colors) / sizeof(colors[0]);

pixel pixels[LED_COUNT];

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // optional console output for validation (set Tools->USB CDC on Boot->Enabled)
  Serial.begin(115200);
  delay(1000);
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);

  Serial.print("Detected ");
  Serial.print(numColors);
  Serial.println(" colors.");  
  
  for(int i=0; i<strip.numPixels(); i++)
  {
    int colorIndex = random(0, numColors - 1);
    int lifetime = random(10,BRIGHTNESS);

    pixels[i].address = i;
    pixels[i].pixelColor = colors[colorIndex];
    pixels[i].lifetime = lifetime;
  }
}

void loop() {
  for(int i=0; i<strip.numPixels(); i++)
  {
      int moveDirectionRoll = random(0,100);

      //Move the pixel address up or down by one.
      if(moveDirectionRoll > 50)
      {
        pixels[i].address = pixels[i].address + 1;
      }
      else
      {
        pixels[i].address = pixels[i].address - 1;
      }
      
      //If the new pixel address is too high or too low, set it to 0.
      if(pixels[i].address > strip.numPixels() - 1 || pixels[i].address <= 0)
      {
        pixels[i].address = 0;
      }

      //The pixel has turned black.
      if(pixels[i].lifetime <= 0)
      {
          //Assign the pixel a new color and lifetime
          int randomColorIndex = random(0, numColors - 1);
          pixels[i].pixelColor = colors[randomColorIndex];
          pixels[i].lifetime = random(10, BRIGHTNESS);
      }

      //Randomly choose whether or not we want the pixel to get brighter or darker.
      int lifeIncreaseRoll = random(0,100);
      int lifetimeDelta = random(2,6);
      if(lifeIncreaseRoll >= 40)
      {
          pixels[i].lifetime = pixels[i].lifetime - lifetimeDelta;
      }
      else
      {
          if(pixels[i].lifetime + lifetimeDelta >= BRIGHTNESS)
          {
            pixels[i].lifetime = pixels[i].lifetime - lifetimeDelta;
          }
          else
          {
            pixels[i].lifetime = pixels[i].lifetime + lifetimeDelta;
          }
      }

      //Set the pixel color after all of this madness.
      strip.setPixelColor(pixels[i].address, (pixels[i].lifetime * pixels[i].pixelColor.R / 255), (pixels[i].lifetime * pixels[i].pixelColor.G / 255), (pixels[i].lifetime * pixels[i].pixelColor.B / 255));
      strip.show();
  }

  delay(random(50,250));
}
