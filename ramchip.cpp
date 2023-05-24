#include "ramchip.h"

unsigned short RamChip256::busReadRequested(unsigned short busAddress) 
{
    if (noRead) return 0;

    if ((busAddress >> 8) == addressUpper) {
        return memory.at(busAddress & 255);
    }
    return 0;
}

void RamChip256::busWriteRequested(unsigned short busAddress, unsigned short busValue)
{
    if (noWrite) return;

    if ((busAddress >> 8) == addressUpper) {
        memory[busAddress & 256] = busValue;
    }
}

unsigned short RamChip256::peek(unsigned char address)
{
    return memory.at(address);
}

void RamChip256::poke(unsigned char address, unsigned short value)
{
    memory[address] = value;
}

void RamChip256::dump() {
    for (int i = 0; i < 256; i += 16) {
        for (int j = i; j < i + 16; j++) {
            std::cout << int_to_hex(memory[j], 4) << " ";
        }
        std::cout << std::endl;
    }
}


void RamChip256::init(std::vector<unsigned short> initialValues)
{
    for (int i = 0; i < initialValues.size(); i++) {
        memory[i] = initialValues[i];
    }
}

RamChip256::RamChip256(unsigned char upper, bool nW, bool nR)
{
    addressUpper = upper;
    noRead = nR;
    noWrite = nW;
    for (int i = 0; i < 256; i++) memory.push_back(0);
}

RamChip256::RamChip256(unsigned char upper)
{
    addressUpper = upper;
    noRead = false;
    noWrite = false;
    for (int i = 0; i < 256; i++) memory.push_back(0);
}

RamChip256::RamChip256()
{
    addressUpper = 0;
    noRead = false;
    noWrite = false;
    for (int i = 0; i < 256; i++) memory.push_back(0);
}