#pragma once

#include "assembler.h"

const std::map<unsigned char, unsigned short> binToMicro1 = std::map<unsigned char, unsigned short>({
    {0b00000, 0b0000000000000000},
    {0b00001, 0b0010000100000000},
    {0b00010, 0b0010000000010000},
    {0b00011, 0b0000000100000010},
    {0b00100, 0b0000001000000001},
    {0b00101, 0b0000011000000000},
    {0b00110, 0b0000100100000000},
    {0b00111, 0b0000000100000000},
    {0b01000, 0b0000000000101000},
    {0b01001, 0b0000100010000000},
    {0b01010, 0b0000001010000000},
    {0b01011, 0b0011000000000000},
    {0b01100, 0b0010000001000000},
    {0b01101, 0b0000000000000000},
    {0b01110, 0b0000000000101000},
    {0b01111, 0b0000000000000100},
    {0b10000, 0b0000100000000100},
    {0b10001, 0b0000100000000100},
    {0b10010, 0b0000100000000100},
    {0b10011, 0b0000100000000100},
    {0b10100, 0b0000100000000100},
    {0b10101, 0b0000000000000100},
    {0b10110, 0b0000000000000100},
    {0b10111, 0b0000100000000100},
    {0b11000, 0b0000000000101000},
    {0b11001, 0b0000000000101000},
    {0b11010, 0b0000000000000000},
    {0b11011, 0b0000000000100010},
    {0b11100, 0b0000001000100000},
    {0b11101, 0b0000000100001000},
    {0b11110, 0b0000000000000100},
    {0b11111, 0b0000000000000000},
    });

const std::map<unsigned char, unsigned short> binToMicro2 = std::map<unsigned char, unsigned short>({
    {0b00000, 0b0000000000000000},
    {0b00001, 0b0000000000000001},
    {0b00010, 0b0000000000000001},
    {0b00011, 0b0000000000000001},
    {0b00100, 0b0000000000000001},
    {0b00101, 0b0000000000000001},
    {0b00110, 0b0000000000000001},
    {0b00111, 0b0000000000000001},
    {0b01000, 0b0000000000000001},
    {0b01001, 0b0000000000000001},
    {0b01010, 0b0000000000000001},
    {0b01011, 0b0000000000000001},
    {0b01100, 0b0000000000000001},
    {0b01101, 0b0000000000010001},
    {0b01110, 0b0000000000100000},
    {0b01111, 0b0000001000000001},
    {0b10000, 0b0000000000000001},
    {0b10001, 0b0001001000000001},
    {0b10010, 0b1000000000000001},
    {0b10011, 0b1111000000000001},
    {0b10100, 0b0110000000000001},
    {0b10101, 0b0000010000000001},
    {0b10110, 0b0000100000000001},
    {0b10111, 0b0000000100000001},
    {0b11000, 0b0000000001000000},
    {0b11001, 0b0000000010000000},
    {0b11010, 0b0000000000000011},
    {0b11011, 0b0000000000000001},
    {0b11100, 0b0000000000000001},
    {0b11101, 0b0000000000000000},
    {0b11110, 0b0001000000000001},
    {0b11111, 0b0000000000001000},
});

const unsigned short instructionLoad1 = 0b1100000000000000;
const unsigned short instructionLoad2 = 0b0000000000000101;

class Processor {
public:
    std::string message1, message2;

    unsigned short accumulator = 0;
    unsigned short mathInput = 0;
    unsigned short inputA = 0;
    unsigned short inputB = 0;
    int result = 0;
    unsigned short gpr[8] = { 0,0,0,0,0,0,0,0 };

    bool zeroFlag = false;
    bool carryFlag = false;

    unsigned short programCounter = 0;
    unsigned short instructionRegister = 0; // NOP, opcode 00000

    unsigned short busAddressRegister = 0;
    unsigned short busAddressLatch = 0;
    unsigned short busInputRegister = 0;
    unsigned short busOutputRegister = 0;

    bool interrupt = false;
    bool readRequest = false;
    bool writeRequest = false;
    unsigned char regSelector = 0;

    bool halt = false;

    Processor(unsigned short pc);
    Processor();

    void raiseInterrupt();
    void run(bool clock);
    void run() { run(!lastClock); }

    void dumpState();
    unsigned char processorState = 0;
private:
    Element document = dbox({
      vbox({
          text("Accumulator Value: "),
          text("Program Counter Value: "),
      }) | border
        });

    Screen screen = Screen::Create(
        Dimension::Full(),
        Dimension::Fit(document)
    );

    int numInstructionsRun = 0;

    bool opLow = false;
    bool addrHigh = false;
    bool regOut = false;
    bool regIn = false;
    bool accOut = false;
    bool accIn = false;
    bool arIn = false;
    bool arLowIn = false;
    bool pcIn = false;
    bool accInLow = false;
    bool addrOut = false;
    bool mathIn = false;
    bool busOut = false;
    bool busIn = false;

    bool aluEnable = false;
    bool carryIn = false;

    bool orLine = false;
    bool fcLine = false;
    bool invALine = false;
    bool invBLine = false;
    bool leftShift = false;
    bool rightShift = false;
    bool cInLine = false;
    bool cRollLine = false;

    void actionStep(unsigned short operand, unsigned char reg);
    bool lastClock = false;
};