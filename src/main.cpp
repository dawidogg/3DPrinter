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
 * and giving feedback through serial port.                       *
 *                                                                *
 * Commands received:                                             *
 * "slurry 1 x1 x2" - tells the printer to spread the first       * 
 *                    slurry between x1 and x2 positions          *
 * "slurry 2 x1 x2" - tells the printer to spread the second      * 
 *                    slurry between x1 and x2 positions          *
 * "uv done"        - tells the printer when to resume            *
 * "next layer"     - tells the printer when a layer is finished  *
 * "printing done"  - brings the printer to idle state            *
 *                                                                *
 * Commands sent:                                                 *
 * "ready"          - communicates about being ready for printing *
 * "uv wait"        - all motors are stopped and the "uv done"    *
 *                    command is awaited                          *
 * "slurry done"    - communicates about finishing one slurry     *
 *                    layer                                       */
#include <Arduino.h>
#include <AccelStepper.h>
#include <Servo.h>

/*------------------------Pin definitions-------------------------*/
// Pin reference: https://reprap.org/wiki/RAMPS_1.4

#define STEPPER_1_STEP 54
#define STEPPER_1_DIR 55
#define STEPPER_1_ENABLE 38

#define STEPPER_2_STEP 60
#define STEPPER_2_DIR 61
#define STEPPER_2_ENABLE 56

#define STEPPER_3_STEP 46
#define STEPPER_3_DIR 48
#define STEPPER_3_ENABLE 62

#define STEPPER_4_STEP 26
#define STEPPER_4_DIR 28
#define STEPPER_4_ENABLE 24

#define SERVO_1 -1
#define SERVO_2 -1

#define SENSOR_LEFT -1
#define SENSOR_RIGHT -1
#define SENSOR_TOP -1
#define SENSOR_BOTTOM -1
#define CALIBRATION_DONE_SENSOR -1

#define PUMP_1 -1
#define PUMP_2 -1


/*---------------------------Constants----------------------------*/
const int SERIAL_SPEED = 9600;
const int HORIZONTAL_STEPPER_SPEED = 300;
const int STEPPER_1_SPEED = 300;
const int STEPPER_2_SPEED = 100;
const int SERVO_SPREAD_DEGREE = 90;
const int SERVO_WIPE_DEGREE = -90;
const int LAYER_THICKNESS = 10;
const int HORIZONTAL_LENGTH = 10000;

/*---------------------------Variables----------------------------*/
int layerLevel = -1;
int slurryTurn;

/*--------------------------Mode settings-------------------------*/

const int MODE_COUNT = 3;

bool calibrationSetup;

enum Mode{
  HOMING,
  CALIBRATION,
  PRINTING
} mode;

enum {
  GO_DOWN, 
  WAIT,
  GO_UP
} calibrationStep;

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

/*-------------------Command structure & parser-------------------*/

const int PARAMETER_COUNT = 4;
struct Command {
  String args[PARAMETER_COUNT];
};

// Function used to process multiparameter commands from serial port
Command readCommandFromSerial(){
  char bufferChar;
  String bufferString;
  Command result;

  for (int i = 0; i < PARAMETER_COUNT; i++) {
    bufferString = "";
    do {
      bufferChar = Serial.read();
      bufferString += bufferChar;
    } while (bufferChar != ' ' || bufferChar != '\n');
    result.args[i] = bufferString;
    if (bufferChar == '\n') return result;
  }
}

/*----------------------Motor definitions-------------------------*/
AccelStepper stepper1(AccelStepper::DRIVER, STEPPER_1_STEP, STEPPER_1_DIR);
AccelStepper stepper2(AccelStepper::DRIVER, STEPPER_2_STEP, STEPPER_2_DIR);
AccelStepper stepper3(AccelStepper::DRIVER, STEPPER_3_STEP, STEPPER_3_DIR);
AccelStepper stepper4(AccelStepper::DRIVER, STEPPER_4_STEP, STEPPER_4_DIR);
Servo servo1, servo2;

/*---------------------------Setup--------------------------------*/
void setup() {
  pinMode(SENSOR_LEFT, INPUT);
  pinMode(SENSOR_RIGHT, INPUT);
  pinMode(SENSOR_TOP, INPUT);
  pinMode(SENSOR_BOTTOM, INPUT);
  pinMode(CALIBRATION_DONE_SENSOR, INPUT);
  pinMode(PUMP_1, OUTPUT);
  pinMode(PUMP_2, OUTPUT);

  stepper1.setEnablePin(STEPPER_1_ENABLE);
  stepper2.setEnablePin(STEPPER_2_ENABLE);
  stepper3.setEnablePin(STEPPER_3_ENABLE);
  stepper4.setEnablePin(STEPPER_4_ENABLE);  
  servo1.attach(SERVO_1);
  servo2.attach(SERVO_2);

  stepper1.setMaxSpeed(STEPPER_1_SPEED);
  stepper2.setMaxSpeed(STEPPER_2_SPEED);
  stepper3.setMaxSpeed(HORIZONTAL_STEPPER_SPEED);
  stepper4.setMaxSpeed(HORIZONTAL_STEPPER_SPEED);
  stepper1.setSpeed(STEPPER_1_SPEED);
  stepper2.setSpeed(STEPPER_2_SPEED);
  stepper3.setSpeed(HORIZONTAL_STEPPER_SPEED);
  stepper4.setSpeed(HORIZONTAL_STEPPER_SPEED);

  calibrationSetup = false;
  
  Serial.begin(SERIAL_SPEED);
}

/*-----------------------------Loop-------------------------------*/
void loop() {
  switch (mode) {
    case HOMING: homing(); break;
    case CALIBRATION: calibration(); break;
    case PRINTING: printing(); break;
    default: mode = HOMING; break;
  }
}

/*------------------Modes in separate functions-------------------*/

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
    Serial.write("ready");
    nextMode();  
    return;
  }
}

void printing() {
  // Waits until a command comes from Raspberry Pi
  while (Serial.available() == 0) continue;
  Command command = readCommandFromSerial();
  
  // "printing done" command
  if (command.args[0] == "printing" && command.args[1] == "done") {
    nextMode();
    return;
  }
  
  // "next layer" command
  if (command.args[0] == "next" && command.args[0] == "layer") {
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
  if (command.args[0] == "slurry") {
    slurryTurn = command.args[1].toInt();
    x1 = command.args[2].toInt();
    x2 = command.args[3].toInt();
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

  Serial.write("uv wait");
  while (Serial.available() == 0) continue;
  command = readCommandFromSerial();
  if (!(command.args[0] == "uv" && command.args[1] == "done")) {
    Serial.write("error at uv");
    return;
  }

  stepper1.runToNewPosition(0);
  stepper2.runToNewPosition(100);
  
  currentStepper->runToNewPosition(x2);
  currentServo->write(SERVO_WIPE_DEGREE);
  currentStepper->runToNewPosition(0);

  Serial.write("slurry done");
}
