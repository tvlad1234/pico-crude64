# pico-crude64
Very basic C64 emulator\
6502 emulation core taken from [here](https://forum.arduino.cc/t/arduino-6502-emulator-basic-interpreter/188328).
## What does it do?
It emulates the original Commodore 64 Basic. Only text mode is supported, with support for colour (text, border and background). PETSCII support is very hacky, but it can run the famous one-liner maze program. Only 32KB of RAM are available, as bank switching isn't implemented.
## Requirements
Raspberry Pi Pico, ST7735-based 128x160 LCD screen and a PS2 keyboard.
## Wiring
For the screen: RST is GPIO16, CS is GPIO17, DC is GPIO20, SCK is GPIO18, TX is GPIO19.\
For the keyboard: DAT is GPIO3, CLK is GPIO2. The keyboard must be connected through a level shifter, otherwise the Pico might get damaged.
## Usage
After starting, you will be asked to select between the C64 or SX-64 ROM. The only significant difference between the two is the color theme of the BASIC editor.\
Pressing _PgUp_ clears the BASIC editor screen.\
Pressing _ESC_ while the emulator is running brings up a menu from where you can reset the emulated C64, load some demo programs or automagically type the color selection _POKE_ commands.
## Pictures
![20220304_182825](https://user-images.githubusercontent.com/60291077/156802646-90c2da56-226b-4580-93fe-46929bd6cd60.jpg)
![20220304_183132](https://user-images.githubusercontent.com/60291077/156802772-0f546fe0-af5a-4b1c-a2e9-4ec6e63c9b6e.jpg)
![20220304_182834](https://user-images.githubusercontent.com/60291077/156802802-95f41bcf-24cc-4ac3-b667-82982613ca89.jpg)
