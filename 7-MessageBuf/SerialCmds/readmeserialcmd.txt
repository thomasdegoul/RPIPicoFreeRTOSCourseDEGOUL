
SerialCmd - FreeRTOS Serial Command Demo for Raspberry Pi Pico

Author: Thomas DEGOUL (adapted from Jon Durrant)
Date: December 10, 2025

 1\. Overview

This program demonstrates the use of FreeRTOS on the Raspberry Pi Pico to control LEDs via serial commands. Four LEDs (GPIO 2, 3, 4, 5) display a binary random value, while a fifth LED (GPIO 6) indicates input/output activity. A random number generator seeded by the Pico's internal temperature avoids repetitive sequences across multiple devices. Runtime statistics are displayed every 3 seconds over USB serial.

The following software agents are used:

  - BlinkAgent: Blinks a status LED (GPIO 25).
  - CounterAgent: Controls the 4 binary LEDs (GPIO 2, 3, 4, 5).
  - DecoderAgent: Decodes serial commands and updates the LEDs.
  - IOAgent: Manages serial I/O and visual activity indication (GPIO 6).

2\. Hardware Requirements

  - Raspberry Pi Pico board (TWO required for Section 9)
  - 5 external LEDs + 220 Ohm current-limiting resistors
  - Jumper wires

Wiring (Local Board):

  - Status LED: GPIO 25 -\> resistor -\> GND
  - LED1: GPIO 2 -\> resistor -\> GND
  - LED2: GPIO 3 -\> resistor -\> GND
  - LED3: GPIO 4 -\> resistor -\> GND
  - LED4: GPIO 5 -\> resistor -\> GND
  - IO Activity LED: GPIO 6 -\> resistor -\> GND

 3\. Software Dependencies

  - Pico SDK (with PICO\_SDK\_PATH configured)
  - FreeRTOS-Kernel (Heap4)
  - CMake (version 3.12 or later)
  - C++17 compatible compiler

4\. Key Features

  - Temperature-seeded random number generation to avoid repetitive sequences.
  - Four independent FreeRTOS tasks with priorities:
  - BlinkAgent (status indication)
  - CounterAgent (binary LED control)
  - DecoderAgent (command decoding)
  - IOAgent (serial I/O and activity LED)
  - Runtime statistics (tasks, stack, heap) printed every 3 seconds.
  - Serial communication for sending commands and receiving acknowledgments.

 5\. Build and Run Instructions

To build and run this project:

1.  Create a build directory:
    mkdir build && cd build
2.  Configure and build:
    cmake .. && make
3.  Flash the .uf2 file to the Pico by holding the BOOTSEL button.

 6\. Expected Output

  - The status LED (GPIO 25) blinks continuously.
  - The 4 binary LEDs (GPIO 2-5) display random values or respond to serial commands.
  - The IO activity LED (GPIO 6) lights up during serial communication.
  - The serial monitor displays system statistics every 3 seconds:

Number of tasks 5
Task: 1 cPri:2 bPri:2 hw:200 MainThread
Task: 2 cPri:2 bPri:2 hw:180 Blink
Task: 3 cPri:2 bPri:2 hw:180 Counter
Task: 4 cPri:2 bPri:2 hw:180 Decode
Task: 5 cPri:3 bPri:3 hw:180 IO Agent
HEAP avl: 12345, blocks 10, alloc: 8, free: 3

 7\. Available Serial Commands

Commands can be sent via USB serial (115200 baud):

  - r: Generate a new random value for the binary LEDs.
  - s\<value\>: Manually set a binary value (e.g., s5 to display 0101).
  - ?: Display available commands.

 8\. Customization

  - Pins: Modify LED1\_PAD, LED2\_PAD, etc. in the source code.
  - Note on specific pins: The current configuration uses GPIO 25 for the status LED (BlinkAgent) and GPIO 6 for the IO Activity LED (IOAgent).
  - Priorities: Adjust TASK\_PRIORITY to change task scheduling.
  - Commands: Extend DecoderAgent to add new features.

