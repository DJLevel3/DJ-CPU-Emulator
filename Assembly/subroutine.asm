// Stack and Subroutine Test
//
// This program tests a software stack implementation and subroutines on that software stack.

// Initialize SP
DEC
TXA R7

// Begin program
IMV #21845

// push
RAL R7   // Put SP in address
STR      // Put accumulator on stack
TXA R6   // Save accumulator for now
RXA R7   // Put SP in accumulator
DEC      // Decrement SP
TXA R7   // Put value back in SP
RXA R6   // Retrieve saved accumulator value

// add 6
TXA R1
IMV #6
ADD R1

// push
RAL R7   // Put SP in address
STR      // Put accumulator on stack
TXA R6   // Save accumulator for now
RXA R7   // Put SP in accumulator
DEC      // Decrement SP
TXA R7   // Put value back in SP
RXA R6   // Retrieve saved accumulator value

// add 1
TXA R1
IMV #1
ADD R1

// push
RAL R7   // Put SP in address
STR      // Put accumulator on stack
TXA R6   // Save accumulator for now
RXA R7   // Put SP in accumulator
DEC      // Decrement SP
TXA R7   // Put value back in SP
RXA R6   // Retrieve saved accumulator value

// prepare operand for function call
IMV #8

// push operand
RAL R7   // Put SP in address
STR      // Put accumulator on stack
TXA R6   // Save accumulator for now
RXA R7   // Put SP in accumulator
DEC      // Decrement SP
TXA R7   // Put value back in SP
RXA R6   // Retrieve saved accumulator value

// push return address
IMT .return1
RAL R7   // Put SP in address
STR      // Put accumulator on stack
TXA R6   // Save accumulator for now
RXA R7   // Put SP in accumulator
DEC      // Decrement SP
TXA R7   // Put value back in SP
RXA R6   // Retrieve saved accumulator value
IAT .doubleminusone
JMP

.return1

// pop 1
RXA R7
INC
TXA R7
RAL R7
LOD

TXA R4

// pop 2
RXA R7
INC
TXA R7
RAL R7
LOD

TXA R3

// pop 3
RXA R7
INC
TXA R7
RAL R7
LOD

TXA R2

// pop 4
RXA R7
INC
TXA R7
RAL R7
LOD

TXA R1

IAT .data1
RXA R1
STR

IAT .data2
RXA R2
STR

IAT .data3
RXA R3
STR

IAT .data4
RXA R4
STR

HLT

.ret
// Pops stack and jumps to it
// Jump directly here to return to execution
RXA R7
INC
TXA R7
RAL R7
LOD
AAL
JMP

// Doubles the operand (which is immediately before the function call on stack)
.doubleminusone
RXA R7
INC
INC
AAL
LOD
TXA R5
ADD R5
DEC
STR
IAT .ret
JMP

MEM #1024
.data1
NOP
.data2
NOP
.data3
NOP
.data4
NOP