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
#define MUX_ADDRESS 0x70
#define OLED_RESET     -1

TwoWire I2Cone = TwoWire(0);
Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT, &I2Cone, OLED_RESET);

// Switch the bus on the tmux - used if we request a bus in 
void switchBus(uint8_t busid) {
  Wire.beginTransmission(0x70);
  Wire.write(1 << busid);
  Wire.endTransmission();
}


void display_init() {
  I2Cone.begin(I2C_SDA, I2C_SCK, (uint32_t) I2C_HZ);
  display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS);
}

void write_display(String data, unsigned bus = 10) {
  if (bus < 8) {

  }
  display.clearDisplay();
  display.dim(true);
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(data);
  display.display();
}

// Muxed 




#endif

/*
#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C  // 128x32
#define OLED_RESET     -1

// We can run this with only one displayobject.
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Switch the bus on the tmux.
void switchBus(uint8_t busid) {
  Wire.beginTransmission(0x70);
  Wire.write(1 << busid);
  Wire.endTransmission();
}

void write(char* sdata, int bus) {
  switchBus(bus);
  oled.clearDisplay();
  oled.setTextSize(3);
  oled.setTextColor(WHITE);
  oled.setCursor(25, 0);
  oled.println(sdata);
  oled.display();
}

void init_displays () {
  for (int i = 0; i<=7; i++) {
    switchBus(i);
    oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  }
}


void setup() {
  init_displays();
  delay(2000);
}

void loop()
{
  write("IAM0", 0);   // 0 bus on the first mux board seems funny, regardless of mux, arduino, pullups or pcb...
  write("IAM1", 1);
  write("IAM2", 2);
  write("IAM3", 3);
  write("IAM4", 4);
  write("IAM5", 5);
  write("IAM6", 6);
  write("IAM7", 7);
  
}
*/