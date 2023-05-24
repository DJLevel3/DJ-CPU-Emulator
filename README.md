# DJ-CPU-Emulator

Yay! It's a program to emulate my CPU!

If you want to run a program with any serious complexity, build for Release with `gui` set to false in `DJ-CPU-Emulator.cpp`. It's way faster than anything else.

The zero page of RAM is from bus address 0x0000 to 0x0ff, and execution begins at 0x0000. Make sure the program is under 256 bytes for now!

The output port is at bus address 0x0100, this will probably change when I flesh out the design better and add more RAM.
