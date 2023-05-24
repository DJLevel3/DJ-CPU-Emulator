// DJ-CPU-Emulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "main.h"

const bool verbose = true;
const bool writeOut = false;
const bool slowDown = false;

const int timescale = 100;

void writeBus(unsigned short address, unsigned short value)
{
    page0.busWriteRequested(address, value);
    std::string str = portOut.busWriteRequested(address, value);
    if (str != "") processor.message1 = str;
    //page1.busWriteRequested(address, value);
    page2.busWriteRequested(address, value);
    page254.busWriteRequested(address, value);
    page255.busWriteRequested(address, value);
}

unsigned short readBus(unsigned short address)
{
    unsigned short bus = page0.busReadRequested(address);
    //bus = bus | page1.busReadRequested(address);
    bus = bus | page2.busReadRequested(address);
    bus = bus | page254.busReadRequested(address);
    bus = bus | page255.busReadRequested(address);
    return bus;
}

void runClock() {
    processor.run(clk);
    if (processor.readRequest) {
        processor.busInputRegister = readBus(processor.busAddressLatch);
        processor.readRequest = false;
    }
    if (processor.writeRequest) {
        writeBus(processor.busAddressLatch, processor.busOutputRegister);
        processor.writeRequest = false;
    }
    clk = !clk;
}

int main()
{

    if (verbose) std::cout << "Setup Start!" << std::endl;
    processor = Processor(0);

    if (verbose) std::cout << "Setup Finish!" << std::endl << "Assembler Start!" << std::endl;

    std::vector<std::string> asmVector;
    std::string asmString("");
    std::ifstream asmFile("div.asm", std::ios_base::in);
    int i = 0;
    if (asmFile.is_open()) {
        while (std::getline(asmFile, asmString)) {
            asmVector.push_back(asmString);
            if (verbose) std::cout << asmString << std::endl;
        }
        asmFile.close();
    }
    else {
        if (verbose) std::cout << "Failed to read file" << std::endl;
        return 1;
    }

    if (verbose) std::cout << std::endl;

    Program prog(asmVector);
    prog.parse();

    if (verbose) {
        std::cout << "Parsed Code: (begins after leading NOPs)" << std::endl;
        prog.printCode();
        std::cout << std::endl << std::endl;
    }

    prog.assemble();

    if (verbose) {
        std::cout << "Assembled Binary: (begins after leading NOPs)" << std::endl;
        prog.printBin();
    }

    if (writeOut) {
        prog.writeFile("test.bin");
        if (verbose) std::cout << "Assembly written!" << std::endl;
    }

    std::vector<unsigned short> binTemp;
    prog.seek(0);
    for (int i = 0; i < prog.size(); i++) {
        binTemp.push_back(prog.nextBin());
    }

    page0.init(binTemp);
    if (verbose) page0.dump();

    while (!processor.halt) {
        runClock();
        if (processor.processorState == 0 && slowDown) {
            std::this_thread::sleep_for(std::chrono::milliseconds(timescale));
            processor.dumpState();
        }
    }

    if (!slowDown) {
        std::cout << std::endl << processor.message1 << std::endl;
        std::cout << processor.message2 << std::endl;
    }

    return 0;
}