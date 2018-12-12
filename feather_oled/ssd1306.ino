#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Define the display object
// The display is connected through I2C
// Required connections are GND, 3V3, SDA and SCL
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

#define BUTTON_A  9
#define BUTTON_B  6
#define BUTTON_C  5

void setup() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  // Delete the following two lines to remove the splashscreen
  display.display();
  delay(1000);

  // Clear the display buffer.
  display.clearDisplay();
  display.display();

  // Use pullup mode, so the inputs
  // don't require a puldown resistor
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

  // Show some text using the display's library
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Hello World!");
  display.print(" The display is working!");
  display.print(" Press any button on");
  display.print(" the FeatherWing");
  display.setCursor(0,0);

  // Update the display
  display.display();
}

void loop() {
  // Print button presses
  if(!digitalRead(BUTTON_A)) display.print("A");
  if(!digitalRead(BUTTON_B)) display.print("B");
  if(!digitalRead(BUTTON_C)) display.print("C");
  delay(10);

  // Update the display
  display.display();
}
