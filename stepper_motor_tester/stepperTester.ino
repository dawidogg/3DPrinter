// İTÜ TAM
// Author: Denis Davidoglu
//
// The following code is used in manual stepper motor controller/tester, 
// where only one motor is active at a time. The code is responsible for 
// reading speed from a binary coded rotary switch and translating rotary 
// encoder's actions into the motor's movement. Switching the signal between
// the available motors is not present, since it is hardware implemented. 

// Rotary encoder pins
#define S1 22
#define S2 23

// Motor driver pins
#define stepPin 32
#define dirPin 50

// Binary coded rotary switch pins
const int rotaryPin[4] {26, 27, 28, 29};

const int speedMultiplier = 10;

int speed;

// Variables needed to read from rotary encoder
int stateS1, prevStateS1;
int updateCount, encoderPos;
 
void setup() {
  Serial.begin(9600);

  updateCount = 0;
  encoderPos = 0;

  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  for (int i = 0; i < 4; i++)
    pinMode(rotaryPin[i], INPUT_PULLUP);
}

void loop() {
  updateSpeed();
  
  // Detect changes in rotary encoder
  stateS1 = digitalRead(S1);
  if (stateS1 != prevStateS1) {
    // Either increments or decrements the encoder's position 
    // based on the applied direction.
    encoderPos += (digitalRead(S1) == digitalRead(S2))*2 - 1;
    
    // Motors are triggered every third time when there are 
    // changes in rotary encoder. My rotary encoder is not reliable
    // and this method improves a little reading the correct direction.
    updateCount++;
    if (updateCount >= 3) {
      updateCount = 0;
      // If encoder's position is positive, this means the overall 
      // change is clockwise and HIGH is written to the direction 
      // pin. If negative, then counterclockwise was intended and 
      // LOW is sent.
      digitalWrite(dirPin, encoderPos > 0);
      // Resets the position for the successive checks
      encoderPos = 0;
      // Moves the motor with desired speed 
      runSpeed();
    }
  }

  prevStateS1 = stateS1;
}

void updateSpeed() {
  int newSpeed = 0;
  // Reading a value from the binary coded rotary switch.
  // Negative logic is implemented.
  for (int i = 0; i < 4; i++)
    newSpeed += (1 << i)*(1 - digitalRead(rotaryPin[i]));
  newSpeed = newSpeed * speedMultiplier;

  if (speed != newSpeed && 
    // For some reason, Arduino sometimes read "15" from 
    // the switch, whereas the maximum was 9. I had to 
    // eliminate this case. 
    newSpeed != ((1 << 4) - 1)*speedMultiplier) {

    speed = newSpeed;
    Serial.print("New speed: ");
    Serial.println(speed);
  }
}

void runSpeed() {
  for (int i = 0; i < speed; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);    
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
}


