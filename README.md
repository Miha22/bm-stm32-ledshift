## STM32F103 Bare-Metal LED Blinking with 74HC595
Practise task based on a low-density stm32f103c6t6 (10KB ram, 32KB flash) according to reference manual https://www.st.com/en/microcontrollers-microprocessors/stm32f103/documentation.html **(rm0008)** in bare metal (registers) style. The setup having 8 outputs *(convenience for masking and bit operation on a byte)* and 1 input for sequence order change. 
### Features
- Bare-metal implementation without any high-level libraries (HAL/cm3).
- Controlled sequential blinking of LEDs in clockwise and counter-clockwise directions based on a button interrupt.
- SPI communication to interface with the 74HC595 shift register.
- Configuration and usage of:
    - System Clock (external crystal 72 MHz configuration with PLL multipliers)
    - Timers for delay and interrupts
    - GPIO pins
    - NVIC interrupt handling
    - SPI peripheral for serial communication
    - Based on the STM32F103C6 documentation.

### Prerequisites

- Hardware:
    - st-link V2 programmer
    - STM32F103C6 microcontroller (SRAM >= 10KB and FLASH >= 32KB)
    - 74HC595 shift register (7 outs)
    - 8>= LEDs with resistors
    - Push-button for interrupts and else things for placement
- Software:
    - GCC for ARM (gcc-arm-none-eabi-6-2017-q2-update)
    - st-flash for programming/debugging (with gdb)
    - Make for build automation

### Setup & Run
#### Hardware Connection

1. Connect STM32F103C6 microcontroller to the 74HC595 shift register using SPI pins:
    - MOSI (PA7) -> Data input of 74HC595
    - SCK (PA5) -> Clock input of 74HC595
    - Latch (PB14) -> Latch input of 74HC595
2. Connect the LEDs to the 74HC595 output pins (Q0–Q7) through appropriate current-limiting resistors 220 Ohms is enough. 22mA is safe for most diy leds.
3. Connect a push-button to PA11 to toggle the direction of the LED sequence.

#### Launch
1. make
2. make flash


