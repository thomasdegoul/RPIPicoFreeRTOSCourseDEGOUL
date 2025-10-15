

MultiBlink - FreeRTOS Multi-LED Blinking Demo for Raspberry Pi Pico
Author: Jon Durrant (adapted by Thomas DEGOUL)
Date: 15-Aug-2022 (updated 2025)



1. Overview
This demo program blinks five LEDs on GPIO pads 0, 2, 3, 4, and 5 using FreeRTOS tasks.
Each LED is controlled by a separate `BlinkAgent` task, with increasing priorities to demonstrate FreeRTOS scheduling.
Runtime statistics are printed every 3 seconds.

2. Hardware Requirements
- Raspberry Pi Pico board
- 5 external LEDs + current-limiting resistors (e.g., 220Ω)
- Jumper wires

Wiring:
- LED 1: Anode to GPIO 0, cathode to GND via resistor.
- LED 2: Anode to GPIO 2, cathode to GND via resistor.
- LED 3: Anode to GPIO 3, cathode to GND via resistor.
- LED 4: Anode to GPIO 4, cathode to GND via resistor.
- LED 5: Anode to GPIO 5, cathode to GND via resistor.

3. Software Dependencies
- Pico SDK
- FreeRTOS-Kernel (Heap4)
- CMake (version 3.12+)
- C++17 compiler



4. Key Features
- Five independent `BlinkAgent` tasks, each blinking an LED.
- Tasks have increasing priorities (TASK_PRIORITY to TASK_PRIORITY + 3).
- Runtime statistics: task list, stack usage, and heap information.
- USB serial output for monitoring.

5. How to Build & Run
Prerequisites:
- Install Pico SDK and set `PICO_SDK_PATH`.
- Ensure FreeRTOS-Kernel is available at `../../lib/FreeRTOS-Kernel`.

Build Steps:
1. Create and enter a build directory:
   ```bash
   mkdir build && cd build
   ```
2. Configure and build:
   ```bash
   cmake .. && make
   ```
3. Flash the `.uf2` file to the Pico.



6. Expected Output
- All five LEDs blink continuously.
- Serial output (USB) shows task states and heap usage every 3 seconds:
  ```
  Number of tasks 6
  Task: 1    cPri:1    bPri:1    hw:200    MainThread
  Task: 2    cPri:2    bPri:2    hw:180    Blink
  Task: 3    cPri:2    bPri:2    hw:180    Worker 1
  Task: 4    cPri:3    bPri:3    hw:180    Worker 2
  Task: 5    cPri:4    bPri:4    hw:180    Worker 3
  Task: 6    cPri:5    bPri:5    hw:180    Worker 4
  HEAP avl: 12345, blocks 10, alloc: 5, free: 2
  ```


7. Customization
- Change LED pins: Edit `LED_PAD`, `LED1_PAD`, etc. in `main.cpp`.
- Adjust priorities: Modify `TASK_PRIORITY` values in `mainTask()`.
- Add more tasks: Extend `vLaunch()` in `main.cpp`.

 8. License
MIT License (unless specified otherwise in dependencies).

