#include <AccelStepper.h>

#define VERTICAL_SENSOR_PIN 1
#define HORIZONTAL_SENSOR_PIN 2

#define PRINTBED_PIN_1 3 
#define PRINTBED_PIN_2 4 
#define PRINTBED_PIN_3 5

#define YSTEPPER_PIN_1 6 
#define YSTEPPER_PIN_2 7 
#define YSTEPPER_PIN_3 8 

#define WIPER_PIN_1 9
#define WIPER_PIN_2 10
#define WIPER_PIN_3 11

const int PRINTBED_SPEED = 50;
const int WIPER_SPEED = 50;
const int YSTEPPER_SPEED = 50;

const int xLimit = 100;
const int yLimit = 4000;
int zLimit = 40000;
const int zDecrement = 10;

int xCurrent;
int yCurrent;
int zCurrent;

AccelStepper printbedM(DRIVER, PRINTBED_PIN_1, PRINTBED_PIN_2, PRINTBED_PIN_3);
AccelStepper ystepperM(DRIVER, YSTEPPER_PIN_1, YSTEPPER_PIN_2, YSTEPPER_PIN_3);
AccelStepper wiperM(DRIVER, WIPER_PIN_1, WIPER_PIN_2, WIPER_PIN_3);

enum {
    CALIBRATION, WIPER_MOVE, YSTEPPER_MOVE, PRINTBED_MOVE, UVFLASH  
} mode;

void nextMode() {
    switch (mode) {
        case CALIBRATION: mode = WIPER_MOVE; break;
        case WIPER_MOVE: mode = YSTEPPER_MOVE; break;
        case YSTEPPER_MOVE: mode = PRINTBED_MOVE; break;
        case PRINTBED_MOVE: mode = UVFLASH; break;
        case UVFLASH: mode = PRINTBED_MOVE; break;
        case PRINTBED_MOVE: mode = WIPER_MOVE; break; 
        default: mode = CALIBRATION; break; 
    }
}

enum Dir {
    FORWARD = 1,
    BACKWARD = -1
};

struct {
    int speed;
    int direction;
    bool calibrated;
    void updateDirection(int d=direction) {
        direction = d;
        printbedM.setSpeed(speed*direction);
    }
    void reverseDirection() {
        direction *= -1;
        printbedM.setSpeed(speed*direction);
    }
} printbedC, wiperC, ystepperC;

void setup() {
    pinMode(VERTICAL_SENSOR_PIN, INPUT);
    pinMode(HORIZONTAL_SENSOR_PIN, INPUT);
    
    pinMode(PRINTBED_PIN_1, OUTPUT);
    pinMode(PRINTBED_PIN_2, OUTPUT);
    pinMode(PRINTBED_PIN_3, OUTPUT);

    pinMode(YSTEPPER_PIN_1, OUTPUT);
    pinMode(YSTEPPER_PIN_2, OUTPUT);
    pinMode(YSTEPPER_PIN_3, OUTPUT);
    
    pinMode(WIPER_PIN_1, OUTPUT);
    pinMode(WIPER_PIN_2, OUTPUT);
    pinMode(WIPER_PIN_3, OUTPUT);

    printbedM.setMaxSpeed(PRINTBED_SPEED);
    ystepperM.setMaxSpeed(YSTEPPER_SPEED);
    wiperM.setMaxSpeed(WIPER_SPEED);
    
    // Setting speed, direction and calibrated
    printbedC = {PRINTBED_SPEED, BACKWARD, false}; // backward for calibration
    ystepperC = {YSTEPPER_SPEED, BACKWARD, false}; // backward for calibration
    wiperC = {WIPER_SPEED, FORWARD, true};

    printbedC.updateDirection();
    ystepperC.updateDirection();
    wiperC.updateDirection();

    mode = CALIBRATION;
}

void calibration() {
    if (digitalRead(VERTICAL_SENSOR_PIN) == 1)
            printbedC.calibrated = true; 
    if (digitalRead(VERTICAL_SENSOR_PIN) == 1)
        ystepperC.calibrated = true; 

    if (!printbedC.calibrated) printbedM.run();
    if (!ystepperC.calibrated) ystepperM.run();

    if (printbedC.calibrated && ystepperC.calibrated) {
        yCurrent = 0;
        zCurrent = 0;
        nextMode();
    }
}

void wiper_move() {
    if (wiperC.direction == FORWARD)
        wiperM.moveTo(xLimit);
    if (wiperC.direction == BACKWARD)
        wiperM.moveTo(0);
    wiperC.reverseDirection();
    nextMode();
}

void ystepper_move() {
    ystepperM.runSpeed();
    yCurrent += ystepperC.direction;
    if ((ystepperC.direction == FORWARD && yCurrent >= yLimit) || 
    (ystepperC.direction == BACKWARD && yCurrent <= 0)) {
        ystepperC.reverseDirection();
        nextMode();
    }
}

void printbed_move() {
    printbedM.runSpeed();
    zCurrent += printbedC.direction;
    if ((printbedC.direction == FORWARD && zCurrent >= zLimit) || 
    (printbedC.direction == BACKWARD && zCurrent <= 0)) {
        printbedC.reverseDirection();
        nextMode();
        if (printbedC.direction == FORWARD)
            zLimit -= zDecrement;
    }
}

void uvflash() {

}

void loop() {
    switch(mode) {
        case CALIBRATION: calibration(); break;
        case WIPER_MOVE: wiper_move(); break;
        case YSTEPPER_MOVE: ystepper_move(); break;
        case PRINTBED_MOVE: printbed_move(); break;
        case UVFLASH: uvflash(); break;
    }
}
