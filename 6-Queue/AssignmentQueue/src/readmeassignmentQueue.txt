 AssignmentQueue - FreeRTOS Binary Random Value Display for Raspberry Pi Pico

Author: Jon Durrant (adapted by Thomas DEGOUL)
Date: August 15, 2022

1. Overview
This demo program uses FreeRTOS to display a 4-bit binary random value on 4 external LEDs (GPIO 2, 3, 4, 5) of the Raspberry Pi Pico. A random value is generated every 3 seconds, and the LEDs light up to represent this binary value. The onboard LED (GPIO 25) blinks continuously to indicate the system is active.

The program also prints runtime statistics (task list, stack usage, heap information) to the USB serial output every 3 seconds.

2. Hardware Requirements
- Raspberry Pi Pico board.
- 4 external LEDs (any color).
- 4 current-limiting resistors (e.g., 220Ω).
- Jumper wires for connections.

LED Wiring:
- LED1: Anode to GPIO 2, cathode to GND via a resistor.
- LED2: Anode to GPIO 3, cathode to GND via a resistor.
- LED3: Anode to GPIO 4, cathode to GND via a resistor.
- LED4: Anode to GPIO 5, cathode to GND via a resistor.
- Onboard LED: GPIO 25 (no additional wiring required).

3. Software Dependencies
- Pico SDK (latest version).
- FreeRTOS-Kernel (Heap4 configuration).
- CMake (version 3.12 or later).
- C++17-compatible compiler.

4. Key Features
- Binary Display: 4 LEDs represent a 4-bit random value (0x0 to 0xF).
- FreeRTOS Tasks: Uses `BlinkAgent` (for the onboard LED) and `CounterAgent` (for the 4 external LEDs).
- Runtime Statistics: Prints task status, stack usage, and heap information every 3 seconds.
- USB Serial Output for real-time monitoring.

5. How to Build and Run

Prerequisites:
- Install the Pico SDK and set the `PICO_SDK_PATH` environment variable.
- Ensure FreeRTOS-Kernel is available in the `../../lib/FreeRTOS-Kernel` directory.

 Build Steps:
1. Create and navigate to a build directory:
   ```bash
   mkdir build && cd build
   ```
2. Configure and build the project:
   ```bash
   cmake .. && make
   ```
3. Flash the generated `.uf2` file to your Raspberry Pi Pico.

 6. Expected Output
- The onboard LED (GPIO 25) blinks continuously.
- The 4 external LEDs (GPIO 2, 3, 4, 5) display a random 4-bit binary value every 3 seconds.
- The USB serial output displays runtime statistics and the current random value.



7. How It Works
- The `mainTask` function initializes a `BlinkAgent` for the onboard LED and a `CounterAgent` for the 4 external LEDs.
- Every 3 seconds, a 4-bit random value is generated (`rand() & 0x0F`) and sent to the `CounterAgent`, which lights up the LEDs to represent this binary value.
- The `runTimeStats` function prints information about task status, stack usage, and memory.


8.The `main` file was updated to enable asynchronous LED pattern control using a FreeRTOS queue:

1. Added a FreeRTOS queue (`xLEDQueue`) to send 4-bit LED patterns from `mainTask` to `CounterAgent`.
2. Modified `mainTask` to:
   - Create the queue (`xLEDQueue = xQueueCreate(5, sizeof(LEDCommand))`).
   - Send a random 4-bit pattern to `CounterAgent` every 3 seconds via the queue (`xQueueSend`).
3. Updated `vLaunch` to initialize the queue before starting tasks.
4. Result: `CounterAgent` blinks the LEDs for 1 second based on the received pattern, while `mainTask` continues generating new patterns every 3 seconds.
The mainTask generates a random 4-bit pattern every 3 seconds and sends it to the CounterAgent via the FreeRTOS queue.

This ensures non-blocking, thread-safe communication between tasks.

