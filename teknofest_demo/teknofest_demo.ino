/* İTÜ TAM                                                          *
 * Author: Denis Davidoglu                                          *
 *                                                                  * 
 * Under the conditions of Teknofest competition, our 3D printer    *
 * will not be able to fully function, and for this reason the      *
 * following code will be used for demonstration. Motors move with  *
 * the same pattern as in real printing. A C-compatible library for *
 * driving stepper motors was developed from scratch and used here. */

// 5 steps - 1 mm
// Horizontal steps: 2850

#include "ITUTAM_Stepper.h"
#include <Arduino.h>
#include <Servo.h> 

// https://reprap.org/wiki/RAMPS_1.4#Pins

// Stepper 3
#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_MIN_PIN           3

// Stepper 4
#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_MIN_PIN          14

// Stepper 1
#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19

// Stepper 2
#define E0_STEP_PIN        26
#define E0_DIR_PIN         28
#define E0_ENABLE_PIN      24

// Servo 1
#define SERVO1_PIN         11
#define SERVO2_PIN         6

motor stepper1 = {"Stepper1", Z_STEP_PIN, Z_DIR_PIN, Z_ENABLE_PIN, X_MIN_PIN, -1, 600, 0};
motor stepper2 = {"Stepper2", E0_STEP_PIN, E0_DIR_PIN, E0_ENABLE_PIN, -1, -1, 1200, 0};
motor stepper3 = {"Stepper3", X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN, X_MIN_PIN, -1, 900, 0};
motor stepper4 = {"Stepper4", Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN, X_MIN_PIN, -1, 900, 0};

int accelerationSteps = 100;

const int printingAreaMin = 400;
const int printingAreaMax = 2400;

Servo servo1, servo2;
const int closedAngle = 180;
const int openAngle = 0;

void homing() {
    findMinMargin(&stepper1);
    stepper1.traversableLength = 8000;
    delay(1000);
    findMinMargin(&stepper3);
    stepper3.traversableLength = printingAreaMax;
    delay(1000);
    findMinMargin(&stepper4);
    stepper4.traversableLength = printingAreaMax;
    delay(1000);
}

void setup() {
    Serial.begin(9600);
    enableDebugMessages = true;
    setupPins(&stepper1);
    setupPins(&stepper2);
    setupPins(&stepper3);
    setupPins(&stepper4);

    servo1.attach(SERVO1_PIN);
    servo2.attach(SERVO2_PIN);
    servo1.write(closedAngle);
    servo2.write(closedAngle);

    stepper2.pos = 0;
    stepper2.traversableLength = 1000;
    homing();
}

bool turn = 1;

void loop() {
    turn = 1 - turn;
    Servo *currentServo;
    motor *currentStepper;

    if (turn == 0) {
        currentServo = &servo1;
        currentStepper = &stepper3;
    }
    if (turn == 1) {
        currentServo = &servo2;
        currentStepper = &stepper4;
    }

    move(currentStepper, printingAreaMax);
    currentServo->write(openAngle);
    delay(2000);
    move(currentStepper, printingAreaMin);
    delay(1000);
    move(currentStepper, 0);
    
    move(&stepper2, stepper2.traversableLength);
    move(&stepper1, stepper1.traversableLength);
    delay(1000);
    move(&stepper1, 0);
    move(&stepper2, 0);

    move(currentStepper, printingAreaMax);
    currentServo->write(closedAngle);
    delay(2000);
    move(currentStepper, 0);
    delay(1000);
}
