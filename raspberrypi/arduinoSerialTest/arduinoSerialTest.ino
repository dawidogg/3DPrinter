String magick(String input) {
    String output;
    char result[1024], size;
    
    size = 0;
    result[0] = 0; // [2*k] - counter
    result[1] = input.charAt(0); // [2*k+1] - number
    
    for (int i = 0; i < input.length(); i++) { 
        if (input.charAt(i) != result[2*size+1]) {
            size++;
            result[2*size] = 1;
            result[2*size+1] = input.charAt(i);
        } else {
            result[2*size]++;
        }
    }

    for (int i = 0; i <= 2*size+1; i++) {
        if (i & 1) 
            output += (char)result[i];
        else
            output += String((int)result[i]);  
    }

    return output;
}

void setup() {
    Serial.begin(115200);
    Serial.println("1");
}

void loop() {
    if (Serial.available()) {
        Serial.println(magick(String(Serial.parseInt())));
    }
    delay(200);
}
