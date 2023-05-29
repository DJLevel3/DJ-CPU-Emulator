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

Interrupts are theoretically implemented now, but there's no way to trigger them yet. I might implement that at some point.

Additionally, the halt instruction completely ends execution here, despite the fact that on the actual processor,
halts only halt execution until an interrupt is triggered. Programs that require this feature will have to replace halts
with short loops that loop until an interrupt is triggered.

-----

## Included Examples, Templates, and Libraries

* `fib.asm` - Example - Fibonacci sequence until overflow, then reverse one to output maximum Fibonacci number
* `pi.asm` - Example - Approximate 1000 * pi by dividing 22000 by 7
* `div.asm` - Example - Divide 65535 by 51
* `subroutine.asm` - Example - Stack and subroutine test
* `stack.asm` - Library - Implementations of a stack (empty-stack descending convention) and subroutine calls/returns


-----

## Recommended Address Mapping for Hardware
* `0x0000 - 0x00ff` Zero Page
  * Required, never omit this section of memory
  * ROM or NVRAM, used for program or constant storage.
  * Writing here is discouraged since on some systems this may be ROM
* `0x0100 - 0x03ff`
  * Optional, add if desired
  * ROM or NVRAM, used for program or constant storage.
  * Writing here is discouraged since on some systems this may be ROM
* `0x0400 - 0x04ff`
  * Required, never omit this section of memory
  * RAM, used for scratch work and general purpose storage
  * All programs are recommended to write here or beyond for all RAM use
* `0x0500 - 0x07ff`
  * Optional, add if desired
  * RAM, used for general purpose storage
* `0x0800 - 0x7fff`
  * Unspecified, use as needed
  * If banked memory is used, it could go here
  * If VRAM is used for a raster display, it could go here
  * If VMEM is used for a vector display, it could go here
  * Not recommended for ports or I/O
* `0x8000`
  * Highly recommended, but not required
  * Output port for general data output
* `0x8001`
  * Highly recommended, but not required
  * Input port for general data input
* `0x8002 - 0xfbff`
  * Unspecified, use as desired
  * Ports and I/O such as controllers and sound hardware should go here
* `0xfc00 - 0xfeff`
  * Optional, add if desired
  * RAM, used for the stack
* `0xff00 - 0xffff`
  * Highly recommended, but not required
  * RAM, used for the stack
  * All programs that use a stack are recommended to start it at `0xffff`

-----

## Recommended Hardware/Software Conventions
These conventions are not required, but they are recommended for program compatibility

* The stack should follow the empty stack convention (SP points to next open address)
* The stack should descend from `0xffff`, meaning the SP is decremented after pushing and incremented after popping.
* Ports and I/O should be located within the upper half of address space, starting at `0x8000`.
* `0x8000` should be a general output and `0x8001` a general input.
* User input from `0x8001` should set an interrupt. Programs should wait for this interrupt before reading.

-----

## Legacy Programs
(These programs not compatible with the current emulator setup or with the hardware conventions)
* `start.asm` - Starts program at arbitrary address and erases initial jump
