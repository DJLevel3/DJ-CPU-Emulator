#pragma once

#include "assembler.h"

// A chip of RAM or ROM, with 256 words of space.
// Addressing is done via a specific upper byte of the bus address,
// with the lower byte being the internal index of the word
class RamChip256 {
public:
    unsigned char addressUpper = 0b00000000;
    bool noWrite = false;
    bool noRead = false;

    unsigned short busReadRequested(unsigned short busAddress);
    std::string busWriteRequested(unsigned short busAddress, unsigned short busValue);

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