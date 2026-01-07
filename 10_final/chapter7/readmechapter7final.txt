

This project is a modified version of the original 4-LED binary counter demo by Jon Durrant. It has been updated and expanded to support 7 LEDs, allowing for binary representation of values from 0 to 127.

Modified by: DEGOUL THOMAS

OVERVIEW

The program runs on the Raspberry Pi Pico using the FreeRTOS real-time operating system. It generates a random 7-bit integer every three seconds, converts it into a JSON string, and then decodes that string to light up the corresponding LEDs in binary format.

The system is organized into several Agents (tasks):

1. MainTask: Generates the random value and manages system statistics.
2. BlinkAgent: Controls a status LED to show the system is alive.
3. CounterAgent: Manages the 7-LED array and handles the binary logic.
4. DecoderAgent: Parses incoming JSON data to extract the count value.

HARDWARE CONFIGURATION

The following GPIO pins are used for the LED outputs:

 Status LED: GPIO 0
 LED 1 (Value 1): GPIO 2
 LED 2 (Value 2): GPIO 3
 LED 3 (Value 4): GPIO 4
 LED 4 (Value 8): GPIO 5
 LED 5 (Value 16): GPIO 15
 LED 6 (Value 32): GPIO 16
LED 7 (Value 64): GPIO 17

 KEY MODIFICATIONS

The original source code was modified in the following ways:

 Increased the COUNT_LEDS constant from 4 to 7.
 Updated the CounterAgent constructor to accept 7 GPIO pins instead of 4.
 Modified the random number mask in the MainTask from 0x0F (15) to 0x7F (127).
 Expanded the LED initialization loop to handle the additional hardware pins.
 Updated the binary masking logic to support the 7th bit.



