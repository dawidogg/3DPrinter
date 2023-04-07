#include "config.h"

// Function used to process multiparameter commands from serial port
// '!' mark indicates the end of command
void readCommandFromSerial(){
  char bufferChar;
  String bufferString;

  for (int i = 0; i < PARAMETER_COUNT; i++) {
    bufferString = "";
    do {
      bufferChar = Serial.read();
      bufferString += bufferChar;
    } while (bufferChar != ' ' || bufferChar != '!');
    command[i] = String(bufferString);
    if (bufferChar == '!') return;
  }
}
