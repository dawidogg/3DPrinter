#include <Servo.h>

#define SERVO1_PIN 11
#define SERVO2_PIN 6
#define SERVO3_PIN 5
#define SERVO4_PIN 4

Servo servo1, servo2, servo3, servo4;

void setup() {
    servo1.attach(SERVO1_PIN);
    servo2.attach(SERVO2_PIN);
    servo3.attach(SERVO3_PIN);
    servo4.attach(SERVO4_PIN);
}

void loop() {
    servo1.write(0);
    servo2.write(0);
    servo3.write(0);
    servo4.write(0);
    delay(1000);
    servo1.write(90);
    servo2.write(90);
    servo3.write(90);
    servo4.write(90);
    delay(1000);
    servo1.write(180);
    servo2.write(180);
    servo3.write(180);
    servo4.write(180);
    delay(1000);
    servo1.write(45);
    servo2.write(45);
    servo3.write(45);
    servo4.write(45);
    delay(1000);
}
