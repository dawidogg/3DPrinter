#include "AccelStepper.h"

const int PIN_1 = 2;
const int PIN_2 = 3;
const int PIN_3 = 4;
const int PIN_4 = 5;
const int TOTAL_STEPS = 300;
AccelStepper stepper(4, PIN_1, PIN_2, PIN_3, PIN_4); 
bool finished;

void setup()
{
  stepper.setMaxSpeed(100);
  stepper.setSpeed(100);
  if (stepper.currentPosition() != 0)
    stepper.moveTo(0);
  finished = false;
}

void loop()
{
  if (stepper.currentPosition() < TOTAL_STEPS) {
    stepper.runSpeed();
  } else if (!finished) {
    Serial.println("Finished!");
    finished = true;
  }
}
