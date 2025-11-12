This program demonstrates using FreeRTOS to display a random binary value on 4 LEDs with the Raspberry Pi Pico.

How It Works:
The program creates two FreeRTOS tasks:
1. A `BlinkAgent` task that blinks an LED on GPIO pin 25 as a visual indicator that the system is active.
2. A `CounterAgent` task that displays a random 4-bit binary value on 4 LEDs (GPIO pins 2, 3, 4, and 5). Each LED represents one bit of a random number ranging from 0x0 to 0xF.

Hardware Requirements:
- Raspberry Pi Pico board
- 5 external LEDs with current-limiting resistors (e.g., 220Ω)
- Jumper wires

Wiring:
- LED 0 (heartbeat indicator): GPIO Pin 25
- LED 1 (least significant bit): GPIO Pin 2
- LED 2: GPIO Pin 3
- LED 3: GPIO Pin 4
- LED 4 (most significant bit): GPIO Pin 5

Features:
- The LED on GPIO 25 blinks regularly to indicate the system is running.
- The other 4 LEDs display a random binary value every 3 seconds.
- The program also prints task statistics and memory usage via USB serial connection.
- The current random value is printed in the serial terminal as "Count R=0xX".

Example Display:
- If the random value is 0x5 (0101 in binary), the LEDs on GPIO pins 2 and 4 will light up, while those on GPIO pins 3 and 5 will remain off.

How to Build and Run:
1. Ensure you have the Pico SDK installed and `PICO_SDK_PATH` configured.
2. Make sure FreeRTOS-Kernel is available at `../../lib/FreeRTOS-Kernel`.
3. Create a `build` directory and compile the project using CMake:
   
   mkdir build && cd build
   cmake .. && make
   
4. Flash the generated `.uf2` file to your Raspberry Pi Pico.

Expected Output:
- LEDs 2 to 5 display a random binary pattern every 3 seconds.
- The LED on GPIO 25 blinks continuously.
- The serial terminal displays information about active tasks, memory usage, and the current random value.

