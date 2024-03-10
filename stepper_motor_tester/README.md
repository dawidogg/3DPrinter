# Stepper Motor Manual Tester
This project was made for Istanbul Technical University's Technologies of Additive Manufacturing (ITU TAM).

<img src="https://github.com/dawidogg/Stepper-Motor-Manual-Tester/blob/main/images/Circuit%20on%20breadboard.jpg?raw=true" width="75%"/>

The board seen above has four A4988 drivers for controlling motors (which can be easily extended to eight), two BCD coded rotary switches for speed and motor selection, and one rotary encoder for manual control. By using a 74HC138 decoder/demultiplexer, only one driver is active at a time, while the others do not consume power. Regardless of the number of motors connected, it was experimentally found that the circuit consumes around 5 W. Arduino Mega was used as a microcontroller; however, only 8 digital pins are required. 

Part of circuit diagram:
![Part of circuit diagram](https://github.com/dawidogg/Stepper-Motor-Manual-Tester/blob/main/images/Part%20of%20circuit%20diagram.png?raw=true)

Full code and schematic files are provided. 
