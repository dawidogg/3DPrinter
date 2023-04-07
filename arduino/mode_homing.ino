#include "config.h"

// Algorithm described by teammates
void homing() {
  while (digitalRead(SENSOR_TOP) == LOW)
    stepper1.runSpeed();
  while (digitalRead(SENSOR_LEFT) == LOW)
    stepper3.runSpeed();
  while (digitalRead(SENSOR_RIGHT) == LOW)
    stepper4.runSpeed();
  if (digitalRead(SENSOR_TOP) && digitalRead(SENSOR_LEFT) && digitalRead(SENSOR_RIGHT)) {
    stepper1.setCurrentPosition(0);
    stepper3.setCurrentPosition(0);
    stepper4.setCurrentPosition(0);
    nextMode();
  }
}
