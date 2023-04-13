/* İTÜ TAM                                                          *
 * Author: Denis Davidoglu                                          */

#include "ITUTAM_Stepper.h"
#include <Arduino.h>

bool enableDebugMessages = false;

double ease_in_ease_out(double x) {
    if (x >= 1) return 1;
    if (x <= 0) return 0;
    double result = pow(x, alpha) / (pow(x, alpha) + pow(1 - x, alpha)) + beta;
    if (result < 0) return 0;
    if (result > 1) return 1;
    return result;
}

void enable(motor *m) {
    digitalWrite(m->e, LOW);
    if (enableDebugMessages) {
        Serial.print(m->name);
        Serial.println(" is enabled");
    }
}

void disable(motor *m) {
    digitalWrite(m->e, HIGH);
    if (enableDebugMessages) {
        Serial.print(m->name);
        Serial.println(" is disabled");
    }
}

void setDirection(motor *m, bool b) {
    digitalWrite(m->dir, b);
    if (enableDebugMessages) {
        Serial.print(m->name);
        Serial.print("'s direction is set to ");
        Serial.println(b);
    }
}

bool getDirection(motor *m) {
    return digitalRead(m->dir);
}

void updatePos(motor *m) {
    if (digitalRead(m->dir) == LOW) m->pos--;
    if (digitalRead(m->dir) == HIGH) m->pos++;
}

void constantMove(motor *m) {
    digitalWrite(m->step, HIGH);
    delayMicroseconds(m->delay);
    digitalWrite(m->step, LOW);
    delayMicroseconds(m->delay);
    updatePos(m);
}

void smoothMove(motor *m, double i) {
    double f = ease_in_ease_out(i/(double)accelerationSteps);
    digitalWrite(m->step, HIGH);
    delayMicroseconds((f+1)*m->delay);
    digitalWrite(m->step, LOW);
    delayMicroseconds((f+1)*m->delay);
    updatePos(m);
}

void accelerate(motor *m) {
    if (enableDebugMessages) {
        Serial.print(m->name);
        Serial.println(" is accelerating");
    }
    for (double i = accelerationSteps; i >= 1; i--)
        smoothMove(m, i);
}

void decelerate(motor *m) {
    if (enableDebugMessages) {
        Serial.print(m->name);
        Serial.println(" is decelerating");
    }
    for (double i = 1; i <= accelerationSteps; i++)
        smoothMove(m, i);
}

void move(motor *m, int target) {
    if (enableDebugMessages) {
        Serial.print(m->name);
        Serial.print(" is moving from ");
        Serial.print(m->pos);
        Serial.print(" to ");
        Serial.println(target);
    }
    enable(m);
    setDirection(m, (m->pos > target)? 0 : 1);
    accelerate(m);
    while (abs(target - m->pos) > accelerationSteps)
        constantMove(m);
    decelerate(m);
    disable(m);
}

void setupPins(motor *m) {
    pinMode(m->step, OUTPUT);  
    pinMode(m->dir, OUTPUT);   
    pinMode(m->e, OUTPUT);
    pinMode(m->min, INPUT);
    pinMode(m->max, INPUT);   
    disable(m);
}

void findMargins(motor *m) {
    if (enableDebugMessages) {
        Serial.print(m->name);
        Serial.println(" is searching for margins");
    }
    const int sensorTrigger = HIGH;

    enable(m);
    setDirection(m, 0);
    while (digitalRead(m->min) != sensorTrigger)
        constantMove(m);
    m->pos = 0;

    if (enableDebugMessages) {
        Serial.print(m->name);
        Serial.println(" found minimum margin");
    }

    setDirection(m, 1);
    accelerate(m);
    while (digitalRead(m->max) != sensorTrigger)
        constantMove(m);
    m->traversableLength = m->pos;
    disable(m);

    if (enableDebugMessages) {
        Serial.print(m->name);
        Serial.println(" found maximum margin");
    }
}
