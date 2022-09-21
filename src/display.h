#ifndef VIPER1306_H
#define VIPER1306_H
#include <Adafruit_SSD1306.h>

#define I2C_HZ 100000
#define I2C_SDA    23
#define I2C_SCK    22
#define I2C_MUX_ADDR 0x70
#define DISPLAY_WIDTH 128 // OLED display width, in pixels
#define DISPLAY_HEIGHT 32 // OLED display height, in pixels
#define DISPLAY_ADDRESS 0x3C  // 128x32
#define OLED_RESET     -1

TwoWire I2Cone = TwoWire(0);
Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT, &I2Cone, OLED_RESET);

void display_init() {
  I2Cone.begin(I2C_SDA, I2C_SCK, (uint32_t) I2C_HZ);
  display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS);
}

void write_display(String data) {
  display.clearDisplay();
  display.dim(true);
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(data);
  display.display();
}

#endif