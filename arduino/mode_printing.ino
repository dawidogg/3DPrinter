#include "config.h"

int slurryTurn = 1;

#ifndef DEMO
void printing() {
  // Waits until a command comes from Raspberry Pi
  while (Serial.available() == 0) continue;
  readCommandFromSerial();
  
  // "printing done" command
  if (command[0] == "printing" && command[1] == "done") {
    nextMode();
    return;
  }
  
  // "next layer" command
  if (command[0] == "next" && command[0] == "layer") {
    layerLevel -= LAYER_THICKNESS;
    return;
  }

  /* Pointers to motors and pump pin. The algorithm is same * 
   * for both materials, for this reason it is written in   *
   * terms of current entities.                             */  
  AccelStepper *currentStepper = NULL;
  Servo *currentServo = NULL;
  int currentPump = -1;

  // Slurry spreading interval limits
  int x1, x2;

  // "slurry 1/2 x1 x2" command
  if (command[0] == "slurry") {
    slurryTurn = command[1].toInt();
    x1 = command[2].toInt();
    x2 = command[3].toInt();
  }

  // Assigning current variables
  if (slurryTurn == 1) {
    currentStepper = &stepper3;
    currentServo = &servo1;
    currentPump = PUMP_1;
  } else if (slurryTurn == 2) {
    currentStepper = &stepper4;
    currentServo = &servo2;
    currentPump = PUMP_2;
  }

  // Algorithm described by teammates

  currentServo->write(SERVO_SPREAD_DEGREE);
  currentStepper->runToNewPosition(x1);
  digitalWrite(currentPump, HIGH);
  
  currentStepper->runToNewPosition(x2);
  digitalWrite(currentPump, LOW);
  
  currentStepper->runToNewPosition(0);
  stepper1.runToNewPosition(layerLevel);

  Serial.write("uv wait!");
  while (Serial.available() == 0) continue;
  readCommandFromSerial();
  if (!(command[0] == "uv" && command[1] == "done")) {
    Serial.write("error at uv!");
    return;
  }

  stepper1.runToNewPosition(0);
  stepper2.runToNewPosition(100);
  
  currentStepper->runToNewPosition(x2);
  currentServo->write(SERVO_WIPE_DEGREE);
  currentStepper->runToNewPosition(0);

  Serial.write("slurry done!");
}
#endif

#ifdef DEMO
void printing() {
  /* Pointers to motors and pump pin. The algorithm is same * 
   * for both materials, for this reason it is written in   *
   * terms of current entities.                             */  
  AccelStepper *currentStepper = NULL;
  Servo *currentServo = NULL;

  // Slurry spreading interval limits
  int x1 = demoX1, x2 = demoX2;
  slurryTurn = 3 - slurryTurn;

  // Assigning current variables
  if (slurryTurn == 1) {
    currentStepper = &stepper3;
    currentServo = &servo1;
  } else if (slurryTurn == 2) {
    currentStepper = &stepper4;
    currentServo = &servo2;
  }

  // Algorithm described by teammates

  currentServo->write(SERVO_SPREAD_DEGREE);
  currentStepper->runToNewPosition(x1);
  
  currentStepper->runToNewPosition(x2);
  
  currentStepper->runToNewPosition(0);
  stepper1.runToNewPosition(layerLevel);

  Serial.write("uv wait!");
  delay(1000);

  stepper1.runToNewPosition(0);
  stepper2.runToNewPosition(100); // ???
  
  currentStepper->runToNewPosition(x2);
  currentServo->write(SERVO_WIPE_DEGREE);
  currentStepper->runToNewPosition(0);

  Serial.write("slurry done!");
}
#endif
