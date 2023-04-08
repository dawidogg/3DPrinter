/* İTÜ TAM                                                        *
 *                                                                *
 * Author: Denis Davidoglu                                        *
 *                                                                *
 * Description: Code for Arduino Mega with RAMPS 1.4 shield used  *
 * for driving motors of 3D printer. The program includes three   *
 * modes: homing, calibration and printing. The first mode works  *
 * autonomously, interacting only with sensors. The second mode   *
 * waits for user confirmation, while the manual calibration is   *
 * being performed. Lastly, in the printing mode Arduino becomes  *
 * governed by an external board, Raspberry Pi, getting commands  *
 * and giving feedback through serial port. All commands' ending  *
 * are delimited with an exclamation mark for parsing simplicity. *
 *                                                                *
 * Commands received:                                             *
 * "slurry 1 x1 x2!"- tells the printer to spread the first       * 
 *                   slurry between x1 and x2 positions           *
 * "slurry 2 x1 x2!"- tells the printer to spread the second      * 
 *                   slurry between x1 and x2 positions           *
 * "uv done!"       - tells the printer when to resume            *
 * "next layer!"    - tells the printer when a layer is finished  *
 * "printing done!" - brings the printer to idle state            *
 *                                                                *
 * Commands sent:                                                 *
 * "ready!"         - communicates about being ready for printing *
 * "uv wait!"       - all motors are stopped and the "uv done"    *
 *                    command is awaited                          *
 * "slurry done!"   - communicates about finishing one slurry     *
 *                    layer                                       */

#include "config.h"

#include <Arduino.h>
#include <AccelStepper.h>
#include <Servo.h>

/*------------------------Global variables------------------------*/
const int PARAMETER_COUNT = 4;
String command[PARAMETER_COUNT];

int layerLevel = -1;

AccelStepper stepper1(AccelStepper::DRIVER, STEPPER_1_STEP, STEPPER_1_DIR);
AccelStepper stepper2(AccelStepper::DRIVER, STEPPER_2_STEP, STEPPER_2_DIR);
AccelStepper stepper3(AccelStepper::DRIVER, STEPPER_3_STEP, STEPPER_3_DIR);
AccelStepper stepper4(AccelStepper::DRIVER, STEPPER_4_STEP, STEPPER_4_DIR);
Servo servo1, servo2;


/*--------------------------Mode settings-------------------------*/
const int MODE_COUNT = 3;

enum Mode{
  HOMING,
  CALIBRATION,
  PRINTING
} mode = PRINTING;

Mode modeOrder[MODE_COUNT][2] = {
  {HOMING, CALIBRATION},
  {CALIBRATION, PRINTING},
  {PRINTING, HOMING}
}; 

void nextMode() {
  for (int i = 0; i < MODE_COUNT; i++)
    if (modeOrder[i][0] == mode) {
      mode = modeOrder[i][1];
      return;
    }
}

/*-----------------------------Loop-------------------------------*/
void loop() {
  switch (mode) {
    case HOMING:  Serial.println("Entering homing mode"); homing(); break;
    case CALIBRATION:  Serial.println("Entering calibration mode"); calibration(); break;
    case PRINTING:  Serial.println("Entering printing mode"); printing(); break;
    default: mode = HOMING; break;
  }
}
