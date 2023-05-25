#include "io.h"

// Handle Output on Bus Write
std::string Output::busWriteRequested(unsigned short busAddress, unsigned short busValue) {
    if (busAddress != this->busAddress) return "";

    std::ostringstream out;
    out << " OUTPUT: " << std::to_string(busValue)
        << " | 0x" << int_to_hex(busValue, 4)
        << " | 0b" << int_to_bin(busValue, 16);
    return out.str();
}

// Handle Input on Bus Read
unsigned short Input::busReadRequested(unsigned short busAddress) {
    if (busAddress != this->busAddress) return 0;

    std::string str;
    std::cin >> str;

    try {
        memory = std::stoi(str);
        return memory;
    }
    catch (...) { return 0; }
}