// Feather M0: MOSI (Data) - 23, SLCK (Clk) - 24
// DC - 10
// RST - 11
// CS - 12

#include <SPI.h>
#include <U8g2lib.h>
#include <Wire.h>

U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, 12, 10, 11);

void setup(void) {
  Serial.begin(9600);
  u8g2.begin();
}

void loop() {
  String message = "Time: " + String(millis() / 1000.0, 1) + "s";

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.drawStr(0, 24, message.c_str());
  u8g2.sendBuffer();
  
  delay(500);
}
