#include <i2c.h>
#include <Adafruit_PWMServoDriver.h> 

#define DRIVER_ADDR 0x40

Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver(DRIVER_ADDR, I2Cone);

const unsigned int flag_servos_hidden[4] = {360, 365, 340, 335};
const unsigned int flag_servos_visible[4] = {220, 530, 180, 500};

void servoSetup(unsigned int bus) {
  switchBus(bus);
  servos.begin();
  servos.setPWMFreq(60);
  servos.setPWM(0, 0, flag_servos_visible[0]);
  servos.setPWM(1, 0, flag_servos_visible[1]);
  servos.setPWM(2, 0, flag_servos_visible[2]);
  servos.setPWM(3, 0, flag_servos_visible[3]);
}

void setServo(unsigned int servo_id, unsigned int newValue, unsigned int bus) {
  switchBus(bus);
  servos.setPWM(servo_id, 0, map(newValue, 0, 65535, flag_servos_hidden[servo_id], flag_servos_visible[servo_id]));
}