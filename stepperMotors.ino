#include <AccelStepper.h>
#include <Servo.h>

#define TOP_SENSOR_PIN -1
#define BOTTOM_SENSOR_PIN -1 
#define HORIZONTAL_SENSOR_PIN -1

#define X_ROT_STEPPER_PIN_1 54 // Step pin
#define X_ROT_STEPPER_PIN_2 55 // Direction pin
#define X_ROT_STEPPER_PIN_3 38 // Enable pin

#define Y_STEPPER_LEFT_PIN_1 60 // Step pin
#define Y_STEPPER_LEFT_PIN_2 61 // Direction pin

#define Y_STEPPER_RIGHT_PIN_1 60 // Step pin
#define Y_STEPPER_RIGHT_PIN_2 61 // Direction pin

#define Z_STEPPER_PIN_1 46 // Step pin
#define Z_STEPPER_PIN_2 48 // Direction pin

#define SERVO_LEFT_PIN -1
#define SERVO_RIGHT_PIN -1

const int X_ROT_STEPPER_SPEED = 50;
const int Y_STEPPER_SPEED = 50;
const int Z_STEPPER_SPEED = 50;
const int servoCloseAngle = 0;
const int servoOpenAngle = 90;

const int xLimit = 100;
const int yLimit = 4000;
int zLimit = 40000;

const int layerThickness = 10;

int xCurrent;
int yCurrent;
int zCurrent;

AccelStepper xRotStepperM(AccelStepper::DRIVER, X_ROT_STEPPER_PIN_1, X_ROT_STEPPER_PIN_2);
AccelStepper yStepperLeftM(AccelStepper::DRIVER, Y_STEPPER_PIN_1, Y_STEPPER_PIN_2);
AccelStepper yStepperRightM(AccelStepper::DRIVER, Y_STEPPER_PIN_1, Y_STEPPER_PIN_2);
AccelStepper zStepperM(AccelStepper::DRIVER, Z_STEPPER_PIN_1, Z_STEPPER_PIN_2);

Servo servoLeft, servoRight;

enum {
    HOMING, CALIBRATION, Y_STEPPER_LEFT_MOVE, Z_STEPPER_MOVE, UVFLASH  
} mode;

enum Dir {
    FORWARD = 1, BACKWARD = -1
};

void nextMode() {
    switch (mode) {
        case HOMING: mode = CALIBRATION; break;
        case CALIBRATION: mode = WIPER_MOVE; break;
        case WIPER_MOVE: mode = Y_STEPPER_MOVE; break;
        case Y_STEPPER_MOVE: mode = Z_STEPPER_MOVE; break;
        case Z_STEPPER_MOVE: mode = UVFLASH; break;
        case UVFLASH: mode = Z_STEPPER_MOVE; break;
        case Z_STEPPER_MOVE: mode = WIPER_MOVE; break; 
        default: mode = HOMING; break; 
    }
}

void setup() {
    pinMode(TOP_SENSOR_PIN, INPUT);
    pinMode(BOTTOM_SENSOR_PIN, INPUT);
    pinMode(HORIZONTAL_SENSOR_PIN, INPUT);
   
    xRotStepperM.setEnablePin(X_ROT_STEPPER_PIN_3);
    xRotStepperM.enableOutputs();

    xRotStepperM.setMaxSpeed(X_ROT_STEPPER_SPEED);
    yLeftStepperM.setMaxSpeed(Y_STEPPER_SPEED);    
    yRightStepperM.setMaxSpeed(Y_STEPPER_SPEED);
    zStepperM.setMaxSpeed(Z_STEPPER_SPEED);

    mode = HOMING;
}

void homing() {
    if (digitalRead(HORIZONTAL_SENSOR_PIN) == HIGH)
        yStepper.homed = true; 
    if (digitalRead(TOP_SENSOR_PIN) == HIGH)
        zStepper.homed = true; 

    if (!yStepper.homed) yStepper.m.runSpeed();
    if (!zStepper.homed) zStepper.m.runSpeed();

    if (yStepper.homed && zStepper.homed) {
        yCurrent = 0;
        zCurrent = 0;
        nextMode();
    }
}

bool calibrationSetup = false;
void calibration() {
    if (!calibrationSetup) {
        xRotStepperM.disableOutputs();
        zStepper.updateDirection(FORWARD);
        calibrationSetup = true;
    }

    zStepper.m.runSpeed();
    if (digitalRead(BOTTOM_SENSOR_PIN) == HIGH) {
        zStepper.updateDirection(BACKWARD);
        xRotStepperM.enableOutputs();
    }

    if (digitalRead(TOP_SENSOR_PIN) == HIGH && zStepper.direction == BACKWARD) {
        zStepper.reverseDirection();
        nextMode();
    }
}

void yLeftStepperMove() {
    yLeftStepperM.runSpeed();
    yLeftCurrent += yStepper.direction;
    if ((yStepper.direction == FORWARD && yCurrent >= yLimit) || 
    (yStepper.direction == BACKWARD && yCurrent <= 0)) {
        yStepper.reverseDirection();
        nextMode();
    }
}

void zStepper.move() {
    zStepper.m.runSpeed();
    zCurrent += zStepper.direction;
    if ((zStepper.direction == FORWARD && zCurrent >= zLimit) || 
    (zStepper.direction == BACKWARD && zCurrent <= 0)) {
        zStepper.reverseDirection();
        nextMode();
        if (zStepper.direction == FORWARD)
            zLimit -= layerThickness;
    }
}

void servoLeftOpen() {
    servoLeft.write(servoOpenAngle);
}

void servoLeftClose() {
    servoLeft.write(servoCloseAngle);
}

void servoRightOpen() {
    servoRight.write(servoOpenAngle);
}

void servoRightClose() {
    servoRight.write(servoCloseAngle);
}

void uvflash() {

}

void loop() {
    switch(mode) {
        case HOMING: homing(); break;
        case CALIBRATION: calibration(); break;
        case Y_STEPPER_MOVE: yStepper.move(); break;
        case Z_STEPPER_MOVE: zStepper.move(); break;
        case UVFLASH: uvflash(); break;
    }
}
