
#define X_MIN_PIN           3
#define X_MAX_PIN           2

void setup() {
    Serial.begin(9600);
    pinMode(X_MIN_PIN, INPUT);
    pinMode(X_MAX_PIN, INPUT);
}

int minprev = 0, maxprev = 0;

void loop() {
    if (digitalRead(X_MIN_PIN) != minprev) {
        minprev = digitalRead(X_MIN_PIN);
        Serial.print("Min pin: ");
        Serial.println(minprev);
    }
    if (digitalRead(X_MAX_PIN) != maxprev) {
        maxprev = digitalRead(X_MAX_PIN);
        Serial.print("Max pin: ");
        Serial.println(maxprev);
    }
}
