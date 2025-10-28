SemaphoreBlink - FreeRTOS LED Blinking Demo with Semaphores for Raspberry Pi Pico
Author: Jon Durrant (adapted by Thomas DEGOUL)
Date: 15-Aug-2022 (updated 2025)

1. Overview
This demo blinks 4 LEDs on GPIO pads 0, 2, 3, and 4 using FreeRTOS tasks. It introduces semaphore synchronization:
- 1 `BlinkAgent` task for LED 0.
- 3 `BlinkWorker` tasks for LEDs 2, 3, and 4, sharing a **counting semaphore** (max count: 3, initial count: 2).
- Runtime statistics are printed every 3 seconds.

2. Hardware Requirements
- Raspberry Pi Pico board
- 4 external LEDs + current-limiting resistors (e.g., 220Ω)
- Jumper wires

Wiring:
- LED 1: Anode to GPIO 0, cathode to GND via resistor.
- LED 2: Anode to GPIO 2, cathode to GND via resistor.
- LED 3: Anode to GPIO 3, cathode to GND via resistor.
- LED 4: Anode to GPIO 4, cathode to GND via resistor.

3. Software Dependencies
- Pico SDK
- FreeRTOS-Kernel (Heap4)
- CMake (version 3.12+)
- C++17 compiler

4. Key Features
- 1 `BlinkAgent` task and 3 `BlinkWorker` tasks.
- Semaphore synchronization for `BlinkWorker` tasks.
- Runtime stats: task list, stack usage, and heap info.
- USB serial output for monitoring.

5. How to Build & Run
Prerequisites:
- Install Pico SDK and set `PICO_SDK_PATH`.
- Ensure FreeRTOS-Kernel is available at `../../lib/FreeRTOS-Kernel`.

Build Steps:
bash
mkdir build && cd build
cmake .. && make

Flash the `.uf2` file to the Pico.

6. Expected Output
- 4 LEDs blink, with `BlinkWorker` tasks synchronized by the semaphore.
- Serial output (USB) shows task states and heap usage every 3 seconds:
  
  Number of tasks 5
  Task: 1    cPri:1    bPri:1    hw:200    MainThread
  Task: 2    cPri:2    bPri:2    hw:180    Blink
  Task: 3    cPri:2    bPri:2    hw:180    Worker 1
  Task: 4    cPri:2    bPri:2    hw:180    Worker 2
  Task: 5    cPri:2    bPri:2    hw:180    Worker 3
  HEAP avl: 12345, blocks 10, alloc: 5, free: 2
  

8. License
MIT License (unless specified otherwise in dependencies).

9. What If We Use 4 Workers and Only 2 Tokens?

If you modify the demo to use 4 BlinkWorker tasks but limit the semaphore to 2 tokens:
Only 2 LEDs can blink simultaneously.
The other 2 workers will block until a token is released.
The system ensures no more than 2 LEDs are active at the same time.
-Required changes:
Add a 4th BlinkWorker for GPIO 4.
Create the semaphore with xSemaphoreCreateCounting(2, 2).
In reality, with very fast response times, LED blinking may appear to overlap slightly due to human perception and task scheduling timing.
