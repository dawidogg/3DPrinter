/* İTÜ TAM                                                          *
 * Author: Denis Davidoglu                                          *
 *                                                                  * 
 * Under the conditions of Teknofest competition, our 3D printer    *
 * will not be able to fully function, and for this reason the      *
 * following code will be used for demonstration. Motors move with  *
 * the same pattern as in real printing. A C-compatible library for *
 * driving stepper motors was developed from scratch and used here. */

#include "ITUTAM_Stepper.h"
#include <Arduino.h>
#include <Servo.h> 

// https://reprap.org/wiki/RAMPS_1.4#Pins

// Stepper 3
#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_MIN_PIN           3
#define X_MAX_PIN           2

// Stepper 4
#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15

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

motor stepper1 = {"Stepper1", Z_STEP_PIN, Z_DIR_PIN, Z_ENABLE_PIN, Z_MIN_PIN, Z_MAX_PIN, 1000, 0};
motor stepper2 = {"Stepper2", E0_STEP_PIN, E0_DIR_PIN, E0_ENABLE_PIN, -1, -1, 1000, 0};
motor stepper3 = {"Stepper3", X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN, X_MIN_PIN, X_MAX_PIN, 300, 0};
motor stepper4 = {"Stepper4", Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN, Y_MIN_PIN, Y_MAX_PIN, 300, 0};

int accelerationSteps = 100;

Servo servo1;

void homing() {
    findMargins(&stepper3);
    //findMargins(&stepper4);
}

void setup() {
    Serial.begin(9600);
    enableDebugMessages = true;
    //setupPins(&stepper1);
    //setupPins(&stepper2);
    setupPins(&stepper3);
    setupPins(&stepper4);
    servo1.attach(SERVO1_PIN);

    servo1.write(0);
    homing();
    //stepper3.pos =  0;
    //stepper3.traversableLength = 500;
    delay(1000);
}

void loop() {
    servo1.write(90);
    delay(200);
    move(&stepper3, stepper3.traversableLength);
    servo1.write(0);
    delay(1000);
    servo1.write(90);
    delay(200);
    move(&stepper3, 0);
    servo1.write(0);
    delay(1000);
    //move(&stepper4, 0);
    //delay(1000);
    //move(&stepper4, stepper4.traversableLength);
    //delay(1000);
    /*move(&stepper1, 0);
    delay(1000);
    move(&stepper1, stepper1.traversableLength);
    delay(1000);*/
}
