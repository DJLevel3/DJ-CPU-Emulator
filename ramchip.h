#pragma once

#include "assembler.h"

class RamChip256 {
public:
    unsigned char addressUpper = 0b00000000;
    bool noWrite = false;
    bool noRead = false;

    unsigned short busReadRequested(unsigned short busAddress);
    void busWriteRequested(unsigned short busAddress, unsigned short busValue);

    unsigned short peek(unsigned char address);
    void poke(unsigned char address, unsigned short value);

    void dump();

    void init(std::vector<unsigned short> initialValues);

    RamChip256(unsigned char upper, bool nW, bool nR);
    RamChip256(unsigned char upper);
    RamChip256();
private:
    std::vector<unsigned short> memory;
};