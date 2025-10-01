# BlinkLEDExt: FreeRTOS LED Blinking on Raspberry Pi Pico
# Files modified and documented by Thomas DEGOUL

---

## Overview
This project demonstrates how to blink an external LED using FreeRTOS on the Raspberry Pi Pico. The LED is connected to GPIO PAD 0. The project uses FreeRTOS tasks for scheduling and the Pico SDK for hardware abstraction.

---

## File Descriptions

### 1. main.cpp
**Purpose:** Main application code for blinking an LED using FreeRTOS.
**Details:**
- Initializes GPIO PAD 0 as an output.
- Creates a FreeRTOS task (`mainTask`) to toggle the LED every 500ms.
- Uses `vTaskDelay` for timing.
- Initializes USB stdio for debug output.
- Starts the FreeRTOS scheduler on core 0.

**Key Functions:**
- `mainTask`: Toggles the LED state.
- `vLaunch`: Creates the FreeRTOS task and starts the scheduler.
- `main`: Initializes hardware and launches the FreeRTOS application.

---

### 2. CMakeLists.txt
**Purpose:** Main build configuration file for the project.
**Details:**
- Sets the project name to `BlinkLEDExt`.
- Includes the Pico SDK and FreeRTOS.
- Links required libraries: `pico_stdlib` and `FreeRTOS-Kernel-Heap4`.
- Configures the build for dual-core RP2040.
- Generates `.uf2`, `.bin`, and `.hex` output files.
- Enables USB stdio and disables UART stdio.
- Uses `picoDeploy.sh` for packaging the release.

---

### 3. FreeRTOS_Kernel_import.cmake
**Purpose:** Locates and imports the FreeRTOS Kernel.
**Details:**
- Searches for the FreeRTOS Kernel path using environment variables or relative paths.
- Adds the FreeRTOS RP2040 port as a subdirectory to the build system.
- Ensures the FreeRTOS Kernel is available for linking.

---

### 4. pico_sdk_import.cmake
**Purpose:** Locates and imports the Raspberry Pi Pico SDK.
**Details:**
- Searches for the Pico SDK path using environment variables or fetches it from Git.
- Includes the Pico SDK initialization script.
- Ensures the Pico SDK is available for the build process.

---

### 5. FreeRTOS_Kernel/ (Directory)
**Purpose:** Contains FreeRTOS configuration files for the RP2040 port.
**Details:**
- Includes the FreeRTOS kernel source and port-specific files.
- Configuration files are located in `port/FreeRTOS-Kernel/`.

---

## Build Instructions

### Prerequisites
- Raspberry Pi Pico SDK
- FreeRTOS Kernel (Heap4 configuration)
- CMake (version 3.12 or higher)
- GCC toolchain for ARM Cortex-M0+

### Steps
1. Set environment variables:
   - `PICO_SDK_PATH`: Path to the Pico SDK.
   - `FREERTOS_KERNEL_PATH`: Path to the FreeRTOS Kernel.

2. Build the project:
   ```bash
   mkdir build && cd build
   cmake .. && make
