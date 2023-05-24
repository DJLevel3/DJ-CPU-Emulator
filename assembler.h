#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdint>
#include <map>
#include <vector>
#include <string>
#include <iomanip>
#include <stdlib.h>

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>\

using namespace ftxui;

const std::map<std::string, bool> noOperand = {
    {"NOP", false},
    {"LOD", false},
    {"STR", false},
    {"CLA", false},
    {"JMP", false},
    {"AAL", false},
    {"CLI", false},
    {"BRI", false},
    {"INC", false},
    {"RSH", false},
    {"LSH", false},
    {"BRZ", false},
    {"BRC", false},
    {"CLF", false},
    {"PJP", false},
    {"AJP", false},
    {"PKA", false},
    {"DEC", false},
    {"HLT", false},
};

const std::map<std::string, unsigned char> opToBin = {
    {"NOP", 0b00000},
    {"IMH", 0b00001},
    {"IML", 0b00010},
    {"LOD", 0b00011},
    {"STR", 0b00100},
    {"TXA", 0b00101},
    {"RXA", 0b00110},
    {"CLA", 0b00111},
    {"JMP", 0b01000},
    {"RAL", 0b01001},
    {"AAL", 0b01010},
    {"IAH", 0b01011},
    {"IAL", 0b01100},
    {"CLI", 0b01101},
    {"BRI", 0b01110},
    {"INC", 0b01111},
    {"ADD", 0b10000},
    {"SUB", 0b10001},
    {"ORR", 0b10010},
    {"AND", 0b10011},
    {"XOR", 0b10100},
    {"RSH", 0b10101},
    {"LSH", 0b10110},
    {"ADC", 0b10111},
    {"BRZ", 0b11000},
    {"BRC", 0b11001},
    {"CLF", 0b11010},
    {"PJP", 0b11011},
    {"AJP", 0b11100},
    {"PKA", 0b11101},
    {"DEC", 0b11110},
    {"HLT", 0b11111}
};

const std::map<unsigned char, std::string> binToOp = {
    {0b00000, "NOP"},
    {0b00001, "IMH"},
    {0b00010, "IML"},
    {0b00011, "LOD"},
    {0b00100, "STR"},
    {0b00101, "TXA"},
    {0b00110, "RXA"},
    {0b00111, "CLA"},
    {0b01000, "JMP"},
    {0b01001, "RAL"},
    {0b01010, "AAL"},
    {0b01011, "IAH"},
    {0b01100, "IAL"},
    {0b01101, "CLI"},
    {0b01110, "BRI"},
    {0b01111, "INC"},
    {0b10000, "ADD"},
    {0b10001, "SUB"},
    {0b10010, "ORR"},
    {0b10011, "AND"},
    {0b10100, "XOR"},
    {0b10101, "RSH"},
    {0b10110, "LSH"},
    {0b10111, "ADC"},
    {0b11000, "BRZ"},
    {0b11001, "BRC"},
    {0b11010, "CLF"},
    {0b11011, "PJP"},
    {0b11100, "AJP"},
    {0b11101, "PKA"},
    {0b11110, "DEC"},
    {0b11111, "HLT"}
};

template< typename T >
std::string int_to_hex(T i, int width)
{
    std::stringstream stream;
    stream << ""
        << std::setfill('0') << std::setw(width)
        << std::hex << i;
    return stream.str();
}

template<typename T>
std::string int_to_bin(T i, int width)
{
    std::string r = "";
    while (i != 0) { r = ((i & 1) ? "1" : "0") + r; i = i >> 1; }
    for (int j = (int)r.size(); j < width; j++) r = "0" + r;
    return r;
}

class Instruction {
public:
    Instruction(std::string opcode, unsigned char reg, unsigned short operand)
    {
        this->opcode = opcode;
        this->reg = reg;
        this->operand = operand;
        encode();
    }
    Instruction(std::string opcode, unsigned char reg) : Instruction(opcode, reg, 0) {};
    Instruction(std::string opcode, unsigned short operand) : Instruction(opcode, 0, operand) {};
    Instruction(std::string opcode) : Instruction(opcode, 0, 0) {};
    Instruction() : Instruction("NOP", 0, 0) {};

    std::string opcode;
    unsigned char reg;
    unsigned short operand;

    void setOpcode(std::string opcode);
    void setReg(unsigned char reg);
    void setOperand(unsigned short operand);

    unsigned short getBin();

    std::string print();
private:
    bool stale = true;
    unsigned short binary;
    void encode();
};

class Program {
public:
    std::vector<std::string> code;

    Program(std::vector<std::string> code);
    Program();

    void setCode(std::vector<std::string> code);
    void setCode(std::string code);

    void appendCode(std::vector<std::string> code);
    void appendCode(std::string code);

    void seek(unsigned short line);
    unsigned short getBin(unsigned short line);
    unsigned short nextBin();

    unsigned short size() { return currentSize; }

    void assemble();
    void parse();

    void printCode();
    void printBin();

    void writeFile(std::string fname);
private:
    unsigned short currentInstruction;
    unsigned short currentSize;

    bool parsed;
    bool assembled;

    std::vector<Instruction> instructions;
    std::map<std::string, unsigned short> tags;
    std::map<unsigned short, std::string> tagsInverse;

    std::vector<unsigned short> binary;
};