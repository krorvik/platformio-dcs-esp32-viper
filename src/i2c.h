#ifndef VIPER1306_H
#define VIPER1306_H
#include <Adafruit_SSD1306.h>

#define I2C_HZ 400000
#define I2C_SDA    23
#define I2C_SCK    22
#define DISPLAY_WIDTH 128 // OLED display width, in pixels
#define DISPLAY_HEIGHT 32 // OLED display height, in pixels
#define DISPLAY_ADDRESS 0x3C  // 128x32
#define MUX_ADDRESS 0x70
#define OLED_RESET     -1

TwoWire I2Cone = TwoWire(0);
Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT, &I2Cone, OLED_RESET);

// Switch the bus on the tmux - used if we request a bus in 
void switchBus(uint8_t busid) {
  I2Cone.beginTransmission(MUX_ADDRESS);
  I2Cone.write(1 << busid);
  I2Cone.endTransmission();
}

void write_display(String data, int x = 0, int y = 0, int textsize = 3) {
  display.clearDisplay();
  display.dim(true);
  display.setTextSize(textsize);
  display.setTextColor(WHITE);
  display.setCursor(x,y);
  display.println(data);
  display.display();
}

void write_display_mux(unsigned bus, String data, int x = 0, int y = 0, int textsize=3) {
  switchBus(bus);
  write_display(data, x, y, textsize);
}

void display_init() {
  I2Cone.begin(I2C_SDA, I2C_SCK, (uint32_t) I2C_HZ);
  display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS);
  display.clearDisplay();
  display.display();
}

void display_init(unsigned frombus, unsigned tobus) {
  I2Cone.begin(I2C_SDA, I2C_SCK, (uint32_t) I2C_HZ);
  for (int i = frombus; i <= tobus; i++) {
    switchBus(i);
    display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS);
    display.clearDisplay();
    display.display();
  }
}

#endif