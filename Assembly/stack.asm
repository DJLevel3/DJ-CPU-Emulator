// DESCRIPTION
// This program (which will actually run, though it doesn't do anything useful) contains copyable code implementing
// a software stack with function calls and returns.

// USAGE
// - To push, pop, or call, copy the whole block directly into your code.
// - To return, jump into .ret. No copy necessary.
//
// - To pass operands to a function, immediately BEFORE calling the function, push operands onto the stack. Inside
//   the function, load the SP and increment it twice to get the address of the newest operand, then increment more
//   to get older operands.
//   * DO NOT store the new stack pointer, this WILL break the return!
//   * DO NOT leave operands on the stack after the return without ensuring they are ALWAYS popped back off, not
//     doing this WILL break subsequent returns!
//   
// - To return data from a function, allocate stack space for the return data before calling the function (such as
//   by pushing 0s or reusing allocations for operands if they can be overwritten) and then pop the appropriate data
//   back off the stack after returning.

// NOTES
// - Push clobbers R6 but saves the accumulator, following the convention that R5 and R6 are caller-saved or volatile registers
// - Pop does not affect any registers, but it overwrites the accumulator with the popped value.
// - Call clobbers R6 since it implements Push
// - Return clobbers R6 but saves the accumulator, following caller-saved convention

// Push
RAL R7       // Put SP in address
STR          // Put accumulator on stack
TXA R6       // Save accumulator for now
RXA R7       // Put SP in accumulator
DEC          // Decrement SP
TXA R7       // Put value back in SP
RXA R6       // Retrieve saved accumulator value

// Pop
RXA R7       // Put SP in accumulator
INC          // Increment SP
TXA R7       // Put value back in SP
RAL R7       // Put SP in address
LOD          // Load from address, SP now points to unallocated memory

// Call
// ===== Push any operands here =====
IMT .resume  // Load return address
             // MAKE SURE THIS IS A UNIQUE TAG!!!
RAL R7       // Put SP in address
STR          // Put accumulator on stack
TXA R6       // Save accumulator for now
RXA R7       // Put SP in accumulator
DEC          // Decrement SP
TXA R7       // Put value back in SP
RXA R6       // Retrieve saved accumulator value
IAT .callee  // Load subroutine's address
JMP          // Jump into subroutine
.resume      // This is where execution will return
             // MAKE SURE THIS MATCHES THE FIRST IMT!!!
// ===== Pop all operands heree =====
HLT

// Return
// Jump here from subroutine to return
.ret
TXA R6       // Save accumulator for now
RXA R7       // Put SP in accumulator
INC          // Increment SP
TXA R7       // Put value back in SP
RAL R7       // Put SP in address
LOD          // Load from address, SP now points to unallocated memory
AAL          // Point jump to return address
RXA R6       // Retrieve saved accumulator value
JMP          // Return to normal execution

// example callee
.callee
NOP          // Your code here
IAT .ret     // Load address of return routine
JMP          // Jump to return routine