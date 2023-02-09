#include <Arduino.h>
#include <AccelStepper.h>
#include <Servo.h>

/*-----------------------Pin definitions-----------------------*/

#define STEPPER_1_STEP 1
#define STEPPER_1_DIR 1
#define STEPPER_1_ENABLE 1

#define STEPPER_2_STEP 1
#define STEPPER_2_DIR 1
#define STEPPER_2_ENABLE 1

#define STEPPER_3_STEP 1
#define STEPPER_3_DIR 1
#define STEPPER_3_ENABLE 1

#define STEPPER_4_STEP 1
#define STEPPER_4_DIR 1
#define STEPPER_4_ENABLE 1

#define STEPPER_5_STEP 1
#define STEPPER_5_DIR 1
#define STEPPER_5_ENABLE 1

#define SERVO_1 1
#define SERVO_2 1

/*-------------------------Mode settings------------------------*/

const int MODE_COUNT = 2;

enum Mode{
  HOMING,
  CALIBRATION
} mode;

Mode modeOrder[MODE_COUNT][2] = {
  {HOMING, CALIBRATION},
  {CALIBRATION, CALIBRATION}
}; 

void nextMode() {
  for (int i = 0; i < MODE_COUNT; i++)
    if (modeOrder[i][0] == mode) {
      mode = modeOrder[i][1];
      return;
    }
}

/*-----------------Command structure & parser-------------------*/

const int PARAMETER_COUNT = 2;
struct Command {
  String commandName;
  String parameters[PARAMETER_COUNT];
};

Command readCommandFromSerial(){
  char bufferChar;
  String bufferString;
  Command result;
  
  // Read command name
  bufferString = "";
  do {
    bufferChar = Serial.read();
    bufferString += bufferChar;
  } while (bufferChar != ' ' || bufferChar != '\n');
  result.commandName = bufferString;
  if (bufferChar == '\n') return result;

  // Read parameters
  for (int i = 0; i < PARAMETER_COUNT; i++) {
    bufferString = "";
    do {
      bufferChar = Serial.read();
      bufferString += bufferChar;
    } while (bufferChar != ' ' || bufferChar != '\n');
    result.parameters[i] = bufferString;
    if (bufferChar == '\n') return result;
  }
}

/*--------------------Motor definitions-------------------------*/

AccelStepper stepper1(AccelStepper::DRIVER, STEPPER_1_STEP, STEPPER_1_DIR);
AccelStepper stepper2(AccelStepper::DRIVER, STEPPER_2_STEP, STEPPER_2_DIR);
AccelStepper stepper3(AccelStepper::DRIVER, STEPPER_3_STEP, STEPPER_3_DIR);
AccelStepper stepper4(AccelStepper::DRIVER, STEPPER_4_STEP, STEPPER_4_DIR);
AccelStepper stepper5(AccelStepper::DRIVER, STEPPER_5_STEP, STEPPER_5_DIR);
Servo servo1, servo2;

/*--------------------------Setup-------------------------------*/

void setup() {
  stepper1.setEnablePin(STEPPER_1_ENABLE);
  stepper2.setEnablePin(STEPPER_2_ENABLE);
  stepper3.setEnablePin(STEPPER_3_ENABLE);
  stepper4.setEnablePin(STEPPER_4_ENABLE);
  stepper5.setEnablePin(STEPPER_5_ENABLE);
  servo1.attach(SERVO_1);
  servo2.attach(SERVO_2);
  Serial.begin(9600);
}

/*---------------------------Loop-------------------------------*/

void loop() {
  switch (mode) {
    case HOMING: homing(); break;
    case CALIBRATION: calibration(); break;
    default: mode = HOMING; break;
  }
}

/*-----------------Modes in separate functions------------------*/

void homing() {

}

void calibration() {

}
