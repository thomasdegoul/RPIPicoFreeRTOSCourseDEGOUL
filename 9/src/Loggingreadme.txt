/***
 * AssignmentQueue - FreeRTOS Logging Framework Demo for Raspberry Pi Pico
 * * Author: Jon Durrant (adapted by Gemini)
 * Date: February 5, 2025
 ***/

1. Overview

This demo program illustrates the use of a Logging Framework operating under FreeRTOS on the Raspberry Pi Pico. The system is structured around multiple concurrent agents (tasks) to demonstrate log management, task synchronization, and real-time resource monitoring. 

The program periodically outputs detailed statistics regarding task states, stack usage, and heap health to the USB serial output, providing a robust template for debugging complex multi-threaded embedded applications.

2. Hardware Requirements

* Raspberry Pi Pico board.
* 1 External LED connected to GPIO 0 (LED_PAD).
* 1 Current-limiting resistor (e.g., 220Ω).
* Jumper wires and breadboard.
* USB Cable for power and Serial UART monitoring.

Wiring:
* LED Anode: Connected to GPIO 0.
* LED Cathode: Connected to GND via the resistor.

3. Software Dependencies

* Pico SDK (latest version).
* FreeRTOS-Kernel (configured for RP2040).
* CMake (version 3.12 or later).
* C++17-compatible compiler.
* Custom Agent headers: BlinkAgent.h, LogAgentA.h, LogAgentB.h.

4. Key Features

* Agent-Oriented Architecture: Uses C++ classes (BlinkAgent, LogAgentA, LogAgentB) to encapsulate FreeRTOS task logic.
* Concurrent Multitasking: Four distinct tasks (MainThread + 3 Agents) running in parallel under the FreeRTOS priority scheduler.
* Real-Time Runtime Statistics: 
    - Monitors Task IDs and Names.
    - Tracks Current and Base Priorities.
    - Stack High Water Mark: Displays the minimum free stack space remaining for each task to prevent overflows.
* Heap Monitoring: Detailed reporting of available heap space, number of free blocks, and successful allocation/free cycles via vPortGetHeapStats.
* USB Serial Output: Real-time system health monitoring at 5-second intervals.

5. How to Build and Run

Prerequisites:
* Ensure PICO_SDK_PATH is set in your environment.
* Ensure the FreeRTOS-Kernel and Agent source files are in the project include path.

Build Steps:
1. Create and navigate to a build directory:
   mkdir build && cd build

2. Configure and build the project:
   cmake .. && make

3. Flash the generated .uf2 file to your Raspberry Pi Pico by holding the BOOTSEL button and dragging the file onto the RPI-RP2 drive.

6. Expected Output

Upon startup, the USB Serial console (e.g., Serial Monitor or Minicom) will display:
1. "GO" followed by "Starting FreeRTOS on core 0:".
2. Every 5 seconds, a table of active tasks:
   - Task: Numerical ID.
   - cPri/bPri: Current and Base priorities.
   - hw: High Water mark (unused stack depth).
   - Name: The string name assigned to the task (e.g., "Blink", "Agent A").
3. A "HEAP avl" report showing remaining memory bytes and block counts.
4. The external LED on GPIO 0 will pulse/blink according to the BlinkAgent logic.

7. How It Works

The system utilizes a non-blocking multitasking design:

1. Initialization (main):
   Initializes the Pico standard I/O and pauses for 2 seconds to allow the USB serial connection to stabilize before launching the scheduler.

2. Main Task (Producer/Monitor):
   Instantiates the BlinkAgent, LogAgentA, and LogAgentB objects. It starts their respective FreeRTOS tasks and then enters a loop to run runTimeStats() every 5000ms.

3. Runtime Statistics (runTimeStats):
   Uses uxTaskGetSystemState to capture a snapshot of all threads. This is vital for identifying "stack-hungry" tasks or priority inversion issues.

4. Log Agents (A & B):
   Run as independent threads, simulating activity and generating logs to demonstrate how the framework handles multiple data sources simultaneously without blocking the main execution flow.

5. Blink Agent:
   An independent task responsible for toggling the GPIO 0 pin, serving as a visual "heartbeat" for the system.
