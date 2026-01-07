README 7 LED FREERTOS SMP COUNTER

This project demonstrates FreeRTOS SMP on the Raspberry Pi Pico. It uses both cores to manage tasks. Core 0 handles system logic and a blinker. Core 1 drives a 7 bit binary counter.

WIRING DIAGRAM
Connect LEDs to these GPIO pins
Blink LED 0 on GPIO 0
Blink LED 1 on GPIO 15
Counter LEDs 1 through 7 on GPIO 2 through 8

CORE DISTRIBUTION
Core 0 runs MainThread and Blink 0
Core 1 runs Counter and Blink 1
Communication uses FreeRTOS Queues to pass data from Core 0 to Core 1

HOW TO BUILD
Initialize a build folder
Run cmake
Run make
Flash the uf2 file to your Pico in BOOTSEL mode

KEY CONFIGURATION
Ensure your FreeRTOSConfig h file includes
configNUM_CORES 2
portSUPPORT_SMP 1


