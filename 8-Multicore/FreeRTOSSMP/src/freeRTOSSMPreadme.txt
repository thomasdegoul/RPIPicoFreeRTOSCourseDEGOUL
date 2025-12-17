AssignmentQueue - FreeRTOS Binary Random Value Display for Raspberry Pi Pico

Author: Jon Durrant (adapted by Thomas DEGOUL)

Date: August 15, 2022

 1. Overview

This demo program uses FreeRTOS to display a 4-bit binary random value on 4 external LEDs (GPIO 2, 3, 4, 5) of the Raspberry Pi Pico. A random value is generated every 3 seconds by the main task and sent asynchronously via a **FreeRTOS Queue** to a counter agent, which lights up the LEDs to represent this binary value. The onboard LED (GPIO 25) blinks continuously to indicate the system is active.

The program also prints runtime statistics (task list, stack usage, heap information) to the USB serial output every 3 seconds.

2. Hardware Requirements

Raspberry Pi Pico board.
4 External LEDs (any color).
4 Current-limiting resistors (e.g., 220Ω).
Jumper wires for connections.

LED Wiring:

LED1: Anode to GPIO 2, cathode to GND via a resistor.
LED2: Anode to GPIO 3, cathode to GND via a resistor.
LED3: Anode to GPIO 4, cathode to GND via a resistor.
LED4: Anode to GPIO 5, cathode to GND via a resistor.
Onboard LED:GPIO 25 (no additional wiring required).

 3. Software Dependencies

Pico SDK (latest version).
FreeRTOS-Kernel (Heap4 configuration).
CMake (version 3.12 or later).
C++17-compatible compiler.

4. Key Features

Binary Display: 4 LEDs represent a 4-bit random value (0x0 to 0xF).
Asynchronous Communication:** Uses a FreeRTOS Queue (`xLEDQueue`) to transfer data between the Main Task and the Counter Agent in a thread-safe manner.
FreeRTOS Tasks: Uses `BlinkAgent` (for the onboard LED) and `CounterAgent` (for the 4 external LEDs).
Runtime Statistics:** Prints task status, stack usage, and heap information every 3 seconds.
USB Serial Output:** Real-time monitoring via standard UART over USB.

5. How to Build and Run

Prerequisites:

Install the Pico SDK and set the `PICO_SDK_PATH` environment variable.
Ensure FreeRTOS-Kernel is available in the `../../lib/FreeRTOS-Kernel` directory (or adjust path in CMakeLists).

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

The Onboard LED (GPIO 25) blinks continuously.
 The 4 External LEDs (GPIO 2, 3, 4, 5) wake up every 3 seconds to display a random 4-bit binary value for a duration of 1 second.
 The USB Serial Output displays runtime statistics (Task list, CPU usage, Heap) and the current random value being sent to the queue.

 7. How It Works

The system is designed around non-blocking inter-task communication:

1. Queue Initialization:
 In `vLaunch`, a FreeRTOS queue `xLEDQueue` is created with a depth of 5 and a generic item size (`sizeof(LEDCommand)`).


2. Main Task (Producer):
 Generates a 4-bit random value (`rand() & 0x0F`) every 3 seconds.
 Sends this value to the `CounterAgent` using `xQueueSend`.
 Prints system health (Runtime stats) to the serial port.


3. Counter Agent (Consumer):
 Monitors `xLEDQueue` for incoming messages.
 Upon receiving a value, it updates the external LEDs to match the binary pattern.
 Holds the pattern for 1 second before waiting for the next command.


4. Blink Agent:
 Runs independently on a separate task to toggle the onboard LED, proving that the scheduler is multitasking effectively.
