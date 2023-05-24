# DJ-CPU-Emulator

Yay! It's a program to emulate my CPU! However, it's unstable, spottily tested, and lacks many QoL features. Beware.

ISA, microcode, and architecture documentation are in `ISA.txt`

If you want to run a program with any serious complexity, build for Release with `gui` set to false in `DJ-CPU-Emulator.cpp`.
It's a lot faster than with `gui` set to true, and WAY faster than building for Debug.

The low pages of RAM are from bus address `0x0000` to `0x07ff`, and execution begins at `0x0000`.
Make sure the program is at most 2048 words for now!

If you want your program to start somewhere other than `0x0000`, I've written `start.asm` which jumps to an arbitrary
memory address and then deletes the jump instructions to leave the start of memory empty.

The output port is at bus address `0x8000`, but this may change later.

Interrupts are theoretically implemented now, but there's no way to trigger them yet. I'll implement that at some point.
