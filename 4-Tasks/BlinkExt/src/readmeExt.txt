

BlinkExt - FreeRTOS LED Blinking Task for Raspberry Pi Pico
Author: Jon Durrant (adapted by Thomas DEGOUL)
Date: 15-Aug-2022 (updated 2025)



1. Overview
This demo program blinks an external LED connected to GPIO PAD 3 on a Raspberry Pi Pico using FreeRTOS. It demonstrates:
- Creating and managing FreeRTOS tasks.
- Using a custom `BlinkAgent` class to control the LED.
- Printing runtime statistics (task list, heap usage) every 3 seconds.



2. Hardware Requirements
- Raspberry Pi Pico board
- External LED + current-limiting resistor (e.g., 220Ω)
- Jumper wires

Wiring:
- Connect the LED anode (long leg) to GPIO 3 (PAD 3).
- Connect the LED cathode (short leg) to GND via the resistor.

---

3. Software Dependencies
- Pico SDK (for Raspberry Pi Pico support)
- FreeRTOS-Kernel (included as a submodule or local path)
- CMake (version 3.12+)
- C++17 compiler

---

4. Project Structure
```
BlinkExt/
├── CMakeLists.txt       # Build configuration
├── src/                 # Source files
│   ├── main.cpp         # Main application
│   └── BlinkAgent.h     # LED control class
├── port/                # FreeRTOS configuration
│   └── FreeRTOS-Kernel/ # FreeRTOS config files
└── lib/                 # FreeRTOS-Kernel library
```

---

5. Key Features
- FreeRTOS Task: Blinks the LED with a priority of `tskIDLE_PRIORITY + 1`.
- Runtime Stats: Prints task and heap statistics every 3 seconds.
- Modular Design: Uses `BlinkAgent` to encapsulate LED logic.



6. How to Build & Run
Prerequisites
- Install [Pico SDK](https://github.com/raspberrypi/pico-sdk) and set `PICO_SDK_PATH`.
- Ensure `FreeRTOS-Kernel` is available at `../../lib/FreeRTOS-Kernel` (or update `CMAKE_CURRENT_LIST_DIR` in `CMakeLists.txt`).

Build Steps
1. Create a build directory:
   ```bash
   mkdir build && cd build
   ```
2. Configure and build:
   ```bash
   cmake .. && make
   ```
3. Flash the `.uf2` file to the Pico:
   - Hold the **BOOTSEL** button, plug in the Pico, and copy `Blink.uf2` to the mounted drive.

---

7. Expected Output
- The LED will blink continuously.
- Serial output (USB) will show:
  - Task list (name, priority, stack usage).
  - Heap statistics (available memory, allocations).

Example output:
```
GO
Starting FreeRTOS on core 0:
Main task started
Number of tasks 3
Task: 1    cPri:1    bPri:1    hw:200    MainThread
HEAP avl: 12345, blocks 10, alloc: 5, free: 2
```



8. Customization
- Change LED Pin: Edit `LED_PAD` in `main.cpp`.
- Adjust Task Priority: Modify `TASK_PRIORITY`.
- Add Tasks: Extend `vLaunch()` in `main.cpp`.


