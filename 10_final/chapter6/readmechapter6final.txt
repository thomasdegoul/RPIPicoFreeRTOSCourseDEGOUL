 README : 7-LED RANDOM BINARY COUNTER

 PROJECT DESCRIPTION

This program is a technical demonstration using a Raspberry Pi Pico and the FreeRTOS real-time operating system. It generates a random 7-bit numerical value and displays it in binary format using a set of 7 Light Emitting Diodes (LEDs). An additional LED is used as a heartbeat monitor to indicate the system is running correctly.

REQUIRED HARDWARE

 1 Raspberry Pi Pico
 7 LEDs for the binary display
 1 LED for the status indicator
 8 Resistors (such as 220 Ohms)
 1 Breadboard and jumper wires

 GPIO PIN CONFIGURATION

Connect your components to the following pins:

 Status LED : GPIO 0
 LED 1 (Bit 0 - Value 1) : GPIO 2
 LED 2 (Bit 1 - Value 2) : GPIO 3
 LED 3 (Bit 2 - Value 4) : GPIO 4
 LED 4 (Bit 3 - Value 8) : GPIO 5
 LED 5 (Bit 4 - Value 16) : GPIO 6
 LED 6 (Bit 5 - Value 32) : GPIO 7
 LED 7 (Bit 6 - Value 64) : GPIO 8

 SOFTWARE ARCHITECTURE

The code is built around two primary agents running as FreeRTOS tasks:

1. BlinkAgent: Manages the status LED on GPIO 0 independently.
2. CounterAgent: Processes commands via a FreeRTOS Queue to display binary values or trigger a blinking effect on the 7 data LEDs.

The main task generates a random integer between 0 and 127 every 3 seconds. This value is then sent to the CounterAgent queue to update the visual display.

INSTALLATION AND BUILD

1. Ensure the Raspberry Pi Pico SDK is properly installed on your computer.
2. Copy the source files into your project directory.
3. Update your CMakeLists.txt file to include the necessary FreeRTOS libraries and headers.
4. Build the project using standard build tools (mkdir build, cd build, cmake .., make).
5. Flash the resulting .uf2 file onto your Pico.

TECHNICAL NOTES

* The range is limited to 7 bits, allowing values from 0 to 127 (0x7F in hexadecimal).
* The system uses FreeRTOS Queues to ensure thread-safe communication between the main task and the LED hardware agent.
* Stack usage and heap statistics are monitored and printed to the serial console for debugging purposes.


