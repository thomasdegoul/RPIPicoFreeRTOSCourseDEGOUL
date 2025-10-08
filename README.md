# BlinkLEDExt: FreeRTOS LED Blinking on Raspberry Pi Pico
**Files modified and documented by Thomas DEGOUL (F14218804)**

---
## Overview
This project demonstrates how to blink an external LED using **FreeRTOS** on the **Raspberry Pi Pico**.
The LED is now connected to **GPIO PAD 3** (changed from GPIO 0).
The project uses **FreeRTOS tasks** for scheduling and the **Pico SDK** for hardware abstraction.

---
## Key Changes This Week
- Changed the LED GPIO from **PAD 0** to **PAD 3** in `main.cpp`.
- Updated the student ID in `CMakeLists.txt` to **F14218804**.
- Powered the LED circuit with **3V** (VCC).
- Adjusted the delay in `mainTask` for runtime stats.

---
## File Descriptions

### 1. `main.cpp`
**Purpose:** Main application code for blinking an LED using FreeRTOS.
**Details:**
- Initializes **GPIO PAD 3** as an output for the external LED.
- Creates a FreeRTOS task (`mainTask`) to toggle the LED and print runtime stats every 3 seconds.
- Uses `BlinkAgent` class to manage LED blinking.
- Initializes USB stdio for debug output.
- Starts the FreeRTOS scheduler on core 0.

**Key Functions:**
- `mainTask`: Starts the `BlinkAgent` and prints task/heap stats.
- `vLaunch`: Creates the FreeRTOS task and starts the scheduler.
- `main`: Initializes hardware and launches the FreeRTOS application.

---

### 2. `CMakeLists.txt`
**Purpose:** Main build configuration file.
**Details:**
- Sets the project name to `Blink` (with student ID comment).
- Includes the Pico SDK and FreeRTOS.
- Links required libraries: `pico_stdlib` and `FreeRTOS-Kernel-Heap4`.
- Configures the build for **dual-core RP2040**.
- Generates `.uf2`, `.bin`, and `.hex` output files.
- Enables **USB stdio** and disables UART stdio.
- Uses `picoDeploy.sh` for packaging the release.

---

### 3. `FreeRTOS_Kernel_import.cmake`
**Purpose:** Locates and imports the FreeRTOS Kernel.
**Details:**
- Searches for the FreeRTOS Kernel path using environment variables or relative paths.
- Adds the FreeRTOS RP2040 port as a subdirectory to the build system.

---

### 4. `pico_sdk_import.cmake`
**Purpose:** Locates and imports the Raspberry Pi Pico SDK.
**Details:**
- Searches for the Pico SDK path using environment variables or fetches it from Git.
- Includes the Pico SDK initialization script.

---

### 5. `FreeRTOS_Kernel/` (Directory)
**Purpose:** Contains FreeRTOS configuration files for the RP2040 port.
**Details:**
- Includes the FreeRTOS kernel source and port-specific files.
- Configuration files are located in `port/FreeRTOS-Kernel/`.

---
## Hardware Setup
- **LED Connection:**
  - Anode to **GPIO 3** (via resistor, e.g., 220Ω).
  - Cathode to **GND**.
- **Power:** LED circuit powered by **3V (VCC)** from the Pico.

---
## Build Instructions

### Prerequisites
- Raspberry Pi Pico SDK
- FreeRTOS Kernel (Heap4 configuration)
- CMake (version 3.12 or higher)
- GCC toolchain for ARM Cortex-M0+

### Steps
1. **Set environment variables:**
   ```sh
   export PICO_SDK_PATH=/path/to/pico-sdk
   export FREERTOS_KERNEL_PATH=/path/to/FreeRTOS-Kernel
