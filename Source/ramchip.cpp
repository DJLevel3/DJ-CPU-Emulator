#include "ramchip.h"

// Read request
unsigned short RamChip256::busReadRequested(unsigned short busAddress) 
{
    if (noRead) return 0;

    if ((busAddress >> 8) == addressUpper) {
        return memory.at(busAddress & 255);
    }
    return 0;
}

// Write request
std::string RamChip256::busWriteRequested(unsigned short busAddress, unsigned short busValue)
{
    if (noWrite) return "";

    if ((busAddress >> 8) == addressUpper) {
        memory[busAddress & 255] = busValue;
        return "Written: " + std::to_string(busValue) + " @ " + std::to_string(busAddress);
    }
    return "";
}

// Peek a specific address at any time
unsigned short RamChip256::peek(unsigned char address)
{
    return memory.at(address);
}

// Modify a specific address at any time
void RamChip256::poke(unsigned char address, unsigned short value)
{
    memory[address] = value;
}

// Dump out the whole contents of the chip in hexadecimal
void RamChip256::dump() {
    for (int i = 0; i < 256; i += 16) {
        for (int j = i; j < i + 16; j++) {
            std::cout << int_to_hex(memory[j], 4) << " ";
        }
        std::cout << std::endl;
    }
}

// Initialize chip with data
void RamChip256::init(std::vector<unsigned short> initialValues)
{
    for (int i = 0; i < std::min((int)initialValues.size(), 256); i++) {
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

RamChip256::RamChip256(unsigned char upper, bool rom)
{
    addressUpper = upper;
    noRead = false;
    noWrite = rom;
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