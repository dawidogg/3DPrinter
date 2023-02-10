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

#define SERVO_1 1
#define SERVO_2 1

#define SENSOR_LEFT 1
#define SENSOR_RIGHT 1
#define SENSOR_TOP 1
#define SENSOR_BOTTOM 1
#define CALIBRATION_DONE_SENSOR 1

/*---------------------------Constants--------------------------*/
const int SERIAL_SPEED = 9600;
const int HORIZONTAL_STEPPER_SPEED = 300;
const int VERTICAL_STEPPER_SPEED = 300;
const int STEPPER_2_SPEED = 100;
const int SERVO_OPEN_DEGREE = 90;
const int SERVO_CLOSED_DEGREE = -90;
const int SLICE_THICKNESS = 10;
const int HORIZONTAL_LENGTH = 10000;

/*---------------------------Variables--------------------------*/
int layerLevel = -1;

/*-------------------------Mode settings------------------------*/

const int MODE_COUNT = 4;

bool calibrationSetup;

enum Mode{
  HOMING,
  CALIBRATION,
  FIRST_SLURRY_PRINT,
  SECOND_SLURRY_PRINT
} mode;

enum {
  GO_DOWN, 
  WAIT,
  GO_UP
} calibrationStep;

Mode modeOrder[MODE_COUNT][2] = {
  {HOMING, CALIBRATION},
  {CALIBRATION, FIRST_SLURRY_PRINT},
  {FIRST_SLURRY_PRINT, SECOND_SLURRY_PRINT},
  {SECOND_SLURRY_PRINT, FIRST_SLURRY_PRINT}
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
Servo servo1, servo2;

/*--------------------------Setup-------------------------------*/

void setup() {
  pinMode(SENSOR_LEFT, INPUT);
  pinMode(SENSOR_RIGHT, INPUT);
  pinMode(SENSOR_TOP, INPUT);
  pinMode(SENSOR_BOTTOM, INPUT);
  pinMode(CALIBRATION_DONE_SENSOR, INPUT);

  stepper1.setEnablePin(STEPPER_1_ENABLE);
  stepper2.setEnablePin(STEPPER_2_ENABLE);
  stepper3.setEnablePin(STEPPER_3_ENABLE);
  stepper4.setEnablePin(STEPPER_4_ENABLE);  
  servo1.attach(SERVO_1);
  servo2.attach(SERVO_2);

  stepper1.setMaxSpeed(VERTICAL_STEPPER_SPEED);
  stepper2.setMaxSpeed(STEPPER_2_SPEED);
  stepper3.setMaxSpeed(HORIZONTAL_STEPPER_SPEED);
  stepper4.setMaxSpeed(HORIZONTAL_STEPPER_SPEED);
  stepper1.setSpeed(VERTICAL_STEPPER_SPEED);
  stepper2.setSpeed(STEPPER_2_SPEED);
  stepper3.setSpeed(HORIZONTAL_STEPPER_SPEED);
  stepper4.setSpeed(HORIZONTAL_STEPPER_SPEED);

  calibrationSetup = false;
  
  Serial.begin(SERIAL_SPEED);
}

/*---------------------------Loop-------------------------------*/

void loop() {
  switch (mode) {
    case HOMING: homing(); break;
    case CALIBRATION: calibration(); break;
    case FIRST_SLURRY_PRINT: print(); break;
    case SECOND_SLURRY_PRINT: print(); break;
    default: mode = HOMING; break;
  }
}

/*-----------------Modes in separate functions------------------*/

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

void calibration() {
  if (!calibrationSetup) {
    stepper1.setSpeed(stepper1.maxSpeed());
    calibrationStep = GO_DOWN;
    calibrationSetup = true;
    return;
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
    nextMode();  
    return;
  }
}

void print() {

}
