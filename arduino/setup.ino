#include "config.h"

void setup() {
  pinMode(SENSOR_LEFT, INPUT);
  pinMode(SENSOR_RIGHT, INPUT);
  pinMode(SENSOR_TOP, INPUT);
  pinMode(SENSOR_BOTTOM, INPUT);
  pinMode(CALIBRATION_DONE_SENSOR, INPUT);
  pinMode(PUMP_1, OUTPUT);
  pinMode(PUMP_2, OUTPUT);

  pinMode(STEPPER_1_ENABLE, OUTPUT);
  pinMode(STEPPER_2_ENABLE, OUTPUT);
  pinMode(STEPPER_3_ENABLE, OUTPUT);
  pinMode(STEPPER_4_ENABLE, OUTPUT);

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
  
  Serial.begin(SERIAL_SPEED);

  stepper1.enableOutputs();
  stepper2.enableOutputs();
  stepper3.enableOutputs();
  stepper4.enableOutputs();
  
  digitalWrite(STEPPER_1_ENABLE, LOW); // Enable driver
  digitalWrite(STEPPER_2_ENABLE, LOW); // Enable driver
  digitalWrite(STEPPER_3_ENABLE, LOW); // Enable driver
  digitalWrite(STEPPER_4_ENABLE, LOW); // Enable driver

#ifdef DEMO
  stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);
  stepper4.setCurrentPosition(0);
  layerLevel = 1000;
  //delay(2000);
  Serial.println("Demo started!");
#endif
}
