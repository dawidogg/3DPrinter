#include "config.h"

enum {
  GO_DOWN, 
  WAIT,
  GO_UP
} calibrationStep;

bool calibrationSetup = false;

// Algorithm described by teammates
void calibration() {
  if (!calibrationSetup) {
    stepper1.setSpeed(stepper1.maxSpeed());
    calibrationStep = GO_DOWN;
    calibrationSetup = true;
  }
  
  if (calibrationStep == GO_DOWN) {
    stepper1.runSpeed();
    if (digitalRead(SENSOR_BOTTOM) == HIGH) {
      stepper1.setSpeed(0);
      layerLevel = stepper1.currentPosition();
      stepper1.setPinsInverted(false, false, true);
      calibrationStep = WAIT;
    }
    return;
  }

  if (calibrationStep == WAIT) {
    if (digitalRead(CALIBRATION_DONE_SENSOR) == HIGH) {
      stepper1.setPinsInverted(false, false, true);
      stepper1.moveTo(0);
      calibrationStep = GO_UP;
    }
    return;
  }

  if (calibrationStep == GO_UP) {
    while (stepper1.distanceToGo() > 0)
      stepper1.run();
    Serial.write("ready!");
    nextMode();  
    return;
  }
}
