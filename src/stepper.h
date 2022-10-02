#ifndef VIPERSTEPPER_H
#define VIPERSTEPPER_H
#include <DcsBios.h>
#include <Button2.h>
#include <FastAccelStepper.h>

#define MAXSTEPPERCOUNT 8
#define SWITCH_PIN 2  // Bottom right iopin (center con) https://external-content.duckduckgo.com/iu/?u=https%3A%2F%2Fesphome.io%2F_images%2Fnodemcu_esp32-full.jpg&f=1&nofb=1
#define PWM_PIN 15      // inner con is PWM input
#define SMOOTH 5
#define PWM_MAX 253 //Slight deadzone
#define MICROSTEPS 8
#define PULSEIN_TIMEOUT 5000

Button2 select_button(SWITCH_PIN);
FastAccelStepperEngine stepper_engine = FastAccelStepperEngine();

const int STEP_PINS[MAXSTEPPERCOUNT] =  { 33, 26, 14, 13, 19,  5, 16, 4 };
const int DIR_PINS[MAXSTEPPERCOUNT]  =  { 32, 25, 27, 12, 21, 18, 17, 0 };

long rotations = 0;
long lastpos = 0;
unsigned int click_counter = 0;
int selected_stepper = 0;
unsigned int steppercount = 0;
bool init_allowed = true;


/*
 * ViperStepper class
 */

class ViperStepper {

  public:
    ViperStepper(        
        unsigned int res, 
        long minval, 
        long maxval, 
        unsigned int speed, 
        unsigned int acceleration,
        unsigned int adjustfactor
    );
    void moveToBounded(long position);
    void moveToContinuous(long position);
    void move(long steps);
    void reset();
    void wait();
    unsigned int adjustFactor;

  private:
    FastAccelStepper *stepper;
    unsigned int resolution;
    long min_value;
    long max_value;
    unsigned int acceleration;
    unsigned int rotations;    
    long last_position;
};

ViperStepper *all_steppers[MAXSTEPPERCOUNT];

ViperStepper::ViperStepper (     
    unsigned int res, 
    long minval, 
    long maxval, 
    unsigned int speed, 
    unsigned int acceleration,
    unsigned int adjustfactor
) {
    this->last_position = 0;
    this->rotations = 0;
    this->resolution = res;
    this->min_value = minval;
    this->max_value = maxval;
    this->adjustFactor = adjustfactor;
    this->stepper = stepper_engine.stepperConnectToPin(STEP_PINS[steppercount]);
    this->stepper->setDirectionPin(DIR_PINS[steppercount]);
    this->stepper->setSpeedInHz(speed * MICROSTEPS);
    this->stepper->setAcceleration(acceleration);
    this->stepper->setCurrentPosition(0);
    // Add to global steppers and count
    all_steppers[steppercount] = this;
    steppercount++;
}

void ViperStepper::wait() {
  while (this->stepper->isRunning()) {};
}

void ViperStepper::reset() {
  this->stepper->stopMove();
  this->wait();
  this->stepper->setCurrentPosition(0);
}

// Can only move to max and back to min
void ViperStepper::moveToBounded(long position) {
  //rawPosition is 0-65535, and we map to our limits
  this->stepper->moveTo(map(position, 0, 65535, min_value * MICROSTEPS, max_value * MICROSTEPS));
}

// No limits, and can rotate many times the same direction (typ: bank indication, compass)
// int signOfX = (x > 0) - (x < 0);
void ViperStepper::moveToContinuous(long position) {
  int diff = position - this->last_position;
  int direction = (diff > 0) - (diff < 0);
  if (abs(diff) >= 32768) {
    diff = 65535 - abs(diff);
    direction = direction * -1;
  }
  last_position = position;
  this->stepper->move(direction * map(diff, 0, 65535, 0, this->resolution * MICROSTEPS));
}

void ViperStepper::move(long steps) {
    this->stepper->move(steps);
}

/*
 * END ViperStepper class
 */

void test_sweep(unsigned stepperid, long max_pos) {
  ViperStepper *stp = all_steppers[stepperid];
  stp->moveToBounded(max_pos);
  stp->wait();
  delay(1000);
  stp->moveToBounded(max_pos / 2);
  stp->wait();
  delay(1000);
  stp->moveToBounded(0);
}

bool initAllowed() {
  return init_allowed;
}

void disable_init() {
  init_allowed = false;
  for (int i = 0; i< steppercount; i++) {
    all_steppers[i]->reset();
  }
}

void enable_init() {
  init_allowed = true;
}

void selector_clickhandler(Button2 &button) {
  if (init_allowed) {
    click_counter += 1;
    selected_stepper = click_counter % (steppercount + 1);
  }
}

void selector_longclickhandler(Button2 &button) {
  if (init_allowed) {
    disable_init();
  } else {
    enable_init();
  }
}

char getStepperID() {
  if(init_allowed and selected_stepper != steppercount) {
    return '0' + selected_stepper;
  }
  // We count one past steppercount to have a "do not adjust mode", return a checkerboard if we're at that point
  return 0xB0;
}

long smoothPWM(){
  if (init_allowed) {        
    long value = 0;  
    for (int i = 0; i < SMOOTH; i++){
        value = value + pulseIn(PWM_PIN, HIGH, PULSEIN_TIMEOUT);
        delay(1);
    }
    return value / SMOOTH;
  }
  return 0;
}

void stepper_init() {
  stepper_engine.init(0); 
  pinMode(PWM_PIN, INPUT);
  select_button.begin(SWITCH_PIN);
  select_button.setClickHandler(selector_clickhandler);
  select_button.setLongClickTime(1000);
  select_button.setLongClickHandler(selector_longclickhandler);

  lastpos = pulseIn(PWM_PIN, HIGH, PULSEIN_TIMEOUT) / 8;
}

long getRotations() {
  return rotations;
}

void adjust() {
  if (init_allowed) {
    // pwm will be 0-2030, devide by 8 for 0-253. )
    long pwm = smoothPWM() / 8;
    long diff = lastpos - pwm;
    if (diff != 0 and abs(diff) < 64 and selected_stepper != steppercount) {
        all_steppers[selected_stepper]->move(0.5 * diff * MICROSTEPS * all_steppers[selected_stepper]->adjustFactor);
    }
    lastpos = pwm;
  }
}

void stepper_loop() {  
  select_button.loop();
  adjust();
}

#endif //VIPERSTEPPER_H

