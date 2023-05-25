#pragma once

#include "assembler.h"
#include <chrono>
#include <thread>

// Template Port
class Port {
public:
    unsigned short busAddress;

    bool noWrite = false;
    bool noRead = false;

    Port(unsigned short address, bool nR, bool nW) { busAddress = address; noRead = nR; noWrite = nW; memory = 0; }
    Port(unsigned short address) { busAddress = address; noRead = false; noWrite = false; memory = 0; }

    unsigned short busReadRequested(unsigned short busAddress) { return 0; }
    std::string busWriteRequested(unsigned short busAddress, unsigned short busValue) { return ""; };
protected:
    unsigned short memory;
};

// Output Writer
class Output : public Port {
    using Port::Port;
public:
    std::string busWriteRequested(unsigned short busAddress, unsigned short busValue);
};

// Input Reader
class Input : public Port {
    using Port::Port;
public:
    unsigned short busReadRequested(unsigned short busAddress);
};