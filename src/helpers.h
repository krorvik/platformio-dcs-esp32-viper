#define m15_limit 49000
#define m1_limit 60413
#define on_limit 62782
const unsigned int m1_ratio = (m1_limit - m15_limit) * 2;
const unsigned int on_ratio = (on_limit - m1_limit) * 2;

char alt_digits[6] = { '0', '0', '0', '0', '0', '\0'};
char baro_digits[5] = {'2', '9', '9', '2', '\0'};
char mach_chars[5] = {'0', '.', '0', '0', '\0'};
char totalizer_digits[6] = { '0', '0', '0', '0', '0', '\0' };
char fuelflow_chars[5] = {'0', '0', '0', '0', '0'};

float calculate_mach(unsigned int value) {  
  if (value >= on_limit) {
    return 0.5;
  } else if (value >= m1_limit) {
    return 0.5 + (on_limit - (float) value) / on_ratio;
  } else {
    return 1 + (m1_limit - (float) value) / m1_ratio;
  }
}

char* alt_checkered() {
  char *tmp_digits = alt_digits;
  if (alt_digits[0] == '0') {
    tmp_digits[0] = 0xB0;
    if (alt_digits[1] == '0' ) { tmp_digits[1] = 0xB0;}
  }
  return tmp_digits;
}

char* fuel_blanked() {
  char *tmp_digits = totalizer_digits;
  if (totalizer_digits[0] == '0') {
    tmp_digits[0] = ' ';
    if (totalizer_digits[1] == '0' ) { tmp_digits[1] = ' ';}
  }
  return tmp_digits;
}

int translateDigit(unsigned int value) {
  if (value < 6553) { return 0;}
  if (value < 13107) { return 1;}
  if (value < 19660) { return 2;}
  if (value < 26214) { return 3;}
  if (value < 32767) { return 4;}
  if (value < 39321) { return 5;}
  if (value < 45874) { return 6;}
  if (value < 52428) { return 7;}
  if (value < 58981) { return 8;}
  return 9;
}

int32_t translate_ias(unsigned int value) {
  if (value <  6553) {
    return (int32_t) map(value,     0,  6553,     0,   460);  // up to 1
  }
  if (value < 13107) {
    return (int32_t) map(value,  6553, 13107,   460,  2280);  // up to 2
  }
  if (value < 16625) {
    return (int32_t) map(value, 13107, 16625,  2280,  2790);  // up to 2.5
  }
  if (value < 19797) {
    return (int32_t) map(value, 16625, 19797,  2790,  3140);  // up to 3
  }
  if (value < 26071) {
    return (int32_t) map(value, 19797, 26071,  3140,  3750);  // up to 4
  }
  if (value < 32975) {
    return (int32_t) map(value, 26071, 32975,  3750,  4210);  // up to 5
  }
  if (value < 39647) {
    return (int32_t) map(value, 32975, 39647,  4210,  4620);  // up to 6
  }
  return (int32_t) map(value, 39647, 58981,  4620,  5760); // up to 9,
}


