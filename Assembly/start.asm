// ========== LEGACY PROGRAM, BREAKS HARDWARE CONVENTIONS ==========
// For this program to work on real hardware, the zero page should be writeable.

// This program allows code to begin at a memory location other than 0x0000 by
// jumping to the appropriate location and then erasing the jump instructions

IAT .erase
JMP

MEM #128

// User program starts here, at the specified address
.program
IMV #42069
IAH #128
IAL #0
STR
CLA
HLT

// Cleanup routine starts here, after the user program
// Once finished, the routine will jump to the user program
.erase
RAL R0
CLA
STR
INC
TXA R1
RAL R1
CLA
STR
IML #2
TXA R1
RAL R1
CLA
TXA R1
STR
IAT .program
JMP