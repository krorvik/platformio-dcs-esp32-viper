#include <U8g2lib.h>
#include <Wire.h>
#include <i2c.h>

const int bubble_min = 7;
const int bubble_max = 121;
// Symbols per https://github.com/olikraus/u8g2/wiki/fnticons
const unsigned int arrow = 0x25bc;
const unsigned int dot = 0x25cf;
const unsigned int pitch_max = 1400;
const unsigned int pitch_min = -1500;
// Limits and center, should be tuned for actual build
const int dev_xmin = -6;
const int dev_xmax = 118;

//U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C bubble_display(U8G2_R0);
U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C bubble_display(U8G2_R0, -1, I2C_SCK, I2C_SDA);
U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C deviation_display(U8G2_R0, -1, I2C_SCK, I2C_SDA);

void adi_display_init(unsigned int deviation_bus, unsigned int bubble_bus) {
    switchBus(deviation_bus);
    deviation_display.begin();
    switchBus(bubble_bus);
    bubble_display.begin();
}

void display_bubble(unsigned int turnrate, unsigned int bubble, unsigned int bus) {
  switchBus(bus);
  bubble_display.firstPage();
  do {
    // Static pattern, set drawcolor to default, since we change it later in this func
    bubble_display.setDrawColor(1);
    bubble_display.drawBox(0,0, 128, 18);
    bubble_display.drawBox(2, 19, 24, 6);
    bubble_display.drawBox(52, 19, 24, 6);
    bubble_display.drawBox(104, 19, 24, 6);
  
    // Draw turn box
    bubble_display.drawBox(map(turnrate, 22768, 42768, 0, 104), 26, 24, 6);
    
    // Set drawColor to 0, 1 is default - invert color for ball
    bubble_display.setDrawColor(0);
    // Static lines for ball center
    bubble_display.drawLine(55, 0, 55, 18);
    bubble_display.drawLine(73, 0, 73, 18);
    // Actual ball
    bubble_display.drawDisc(map(bubble, 0, 65535, bubble_min, bubble_max),9,7);
    
  } while ( bubble_display.nextPage() );
}

void display_deviation(unsigned int deviation, unsigned int bus) {
  switchBus(bus);
  // Calculate where and what to draw
  unsigned int symbol_position = map(deviation, 0, 65535, dev_xmin, dev_xmax);
  unsigned int symbol = arrow;
  if (symbol_position <= 58 and symbol_position >= 55) {
    symbol = dot;
  }
  deviation_display.firstPage();
  do {
    deviation_display.setFont(u8g2_font_cu12_t_symbols);
    deviation_display.drawGlyph(symbol_position, 12, symbol);
  } while ( deviation_display.nextPage() );    
}

