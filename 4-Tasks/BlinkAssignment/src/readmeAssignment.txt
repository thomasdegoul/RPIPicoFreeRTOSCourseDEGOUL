

BlinkAssignment - FreeRTOS Multi-Task LED Blinking Demo for Raspberry Pi Pico
Author: Jon Durrant (adapted by Thomas DEGOUL)
Date: 15-Aug-2022 (updated 2025)


1. Overview
This demo program runs three LED blinking tasks with assigned priorities on a Raspberry Pi Pico using FreeRTOS.
- BlinkAgent: Blinks LED on GPIO 0 (highest priority: TASK_PRIORITY + 3).
- BlinkWorker: Blinks LED on GPIO 2 (medium priority: TASK_PRIORITY + 1) with simulated work.
- BlinkHeavy: Blinks LED on GPIO 3 (high priority: TASK_PRIORITY + 2) with heavy simulated work.
It demonstrates FreeRTOS task prioritization, scheduling, and runtime statistics.



2. Hardware Requirements
- Raspberry Pi Pico board
- 3 external LEDs + current-limiting resistors (e.g., 220Ω)
- Jumper wires

Wiring:
- LED 1: Anode to GPIO 0, cathode to GND via resistor.
- LED 2: Anode to GPIO 2, cathode to GND via resistor.
- LED 3: Anode to GPIO 3, cathode to GND via resistor.



3. Software Dependencies
- Pico SDK
- FreeRTOS-Kernel
- CMake (version 3.12+)
- C++17 compiler



 4. Key Features
- Three tasks with assigned priorities to observe scheduling behavior.
- Runtime statistics printed every 3 seconds (task list, stack usage, heap usage).
- Simulated workloads to demonstrate task execution patterns.



5. How to Build & Run
Prerequisites:
- Install Pico SDK and set PICO_SDK_PATH.
- Ensure FreeRTOS-Kernel is available at ../../lib/FreeRTOS-Kernel.

Build Steps:
1. Create and enter a build directory:
   mkdir build && cd build
2. Configure and build:
   cmake .. && make
3. Flash the .uf2 file to the Pico.



6. Expected Output
- LEDs blink according to task priorities and workloads.
- Serial output shows task states and heap usage:
  Number of tasks 4
  Task: 1    cPri:1    bPri:1    hw:200    MainThread
  Task: 2    cPri:4    bPri:4    hw:180    Blink
  Task: 3    cPri:2    bPri:2    hw:180    Worker 1
  Task: 4    cPri:3    bPri:3    hw:150    Worker 2
  HEAP avl: 12345, blocks 10, alloc: 5, free: 2



 7. Customization
- Change LED pins: Edit LED_PAD, LED1_PAD, LED2_PAD in main.cpp.
- Adjust priorities: Modify TASK_PRIORITY values in mainTask().
- Add more tasks: Extend vLaunch() in main.cpp.



 8. License
MIT License (unless specified otherwise in dependencies).

---
