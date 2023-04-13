/* İTÜ TAM                                                          *
 * Author: Denis Davidoglu                                          *
 *                                                                  *
 * This is an independetly developed C-compatible Arduino library   *
 * for driving stepper motors with acceleration and deceleration.   *
 * Currently not tested.                                            */

#include <Arduino.h>

#ifndef TAM_STEP
#define TAM_STEP

// Ease-in ease-out parameters.
const int alpha = 2, beta = 0.07;
// Number of iterations to accelerate/decelerate.
const int accelerationSteps = 50;

typedef struct {
    // Name that will be shown in debug messages.
    const char* name;
    // Step pin.
    const int step;
    // Direction pin.
    const int dir;
    // Enable pin.
    const int e;
    // Margin sensor (min).
    const int min;
    // Margin sensor (max).
    const int max;
    // Delay between step pin impulses at constant speed.
    int delay;
    // Current position.
    int pos;
    // Distance between zero position (min) and the farthest position (max).
    int traversableLength;
} motor;

// Set as true to view debug messages.
extern bool enableDebugMessages;

// Function which takes a value between [0, 1] and returns a value between [0, 1], used for acceleration and deceleration.
double ease_in_ease_out(double);
void enable(motor*);
void disable(motor*);
void setDirection(motor*, bool);
bool getDirection(motor*);
// Either increments or decrements position varialbe, according to the direction.
void updatePos(motor*);
// Sends a single high-low pulse to motor with constant frequency.
void constantMove(motor*);
// Sends a single high-low pulse to motor with frequency regulated by ease_in_ease_out function.
void smoothMove(motor*, double); 
void accelerate(motor*);
void decelerate(motor*);
// Moves motor to desired position.
void move(motor*, int);
// Sets modes as input and output for pins which define motor.
void setupPins(motor*);
// Can be called in the beginning of execution to find the zero position and distance between min and max sensors.
void findMargins(motor*);

#endif