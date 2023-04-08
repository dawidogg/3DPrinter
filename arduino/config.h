#ifndef CONFIG
#define CONFIG

// DEMO is for testing and demonstration purposes.
// Comment the definition to compile full code.
#define DEMO

// X - connected
#define STEPPER_1_STEP 54
#define STEPPER_1_DIR 55
#define STEPPER_1_ENABLE 38

// Y - connected
#define STEPPER_2_STEP 60
#define STEPPER_2_DIR 61
#define STEPPER_2_ENABLE 56

// Z
#define STEPPER_3_STEP 46
#define STEPPER_3_DIR 48
#define STEPPER_3_ENABLE 62

// E0 - connected
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

const int SERIAL_SPEED = 9600;
const int HORIZONTAL_STEPPER_SPEED = 200;
const int STEPPER_1_SPEED = 100;
const int STEPPER_2_SPEED = 100;
const int SERVO_SPREAD_DEGREE = 90;
const int SERVO_WIPE_DEGREE = -90;
const int LAYER_THICKNESS = 100;
const int HORIZONTAL_LENGTH = 1000;

#ifdef DEMO
const int demoX1 = 50;
const int demoX2 = 400;
#endif

#endif
