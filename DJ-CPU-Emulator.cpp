// DJ-CPU-Emulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "main.h"

// Output the parsed code, assembled binary, and contents of zero page of RAM
const bool verbose = false;

// Write assembly file
const bool writeOut = false;

// Use GUI to display processor internals
const bool gui = false;

// Slow down execution
const bool slowDown = false;

// How much to wait between instructions
const int timescale = 1;

// Filename to read .asm and write .bin
const std::string file = "div";

// Bus write event
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

// Bus read event
unsigned short readBus(unsigned short address)
{
    unsigned short bus = page0.busReadRequested(address);
    //bus = bus | page1.busReadRequested(address);
    bus = bus | page2.busReadRequested(address);
    bus = bus | page254.busReadRequested(address);
    bus = bus | page255.busReadRequested(address);
    return bus;
}

// Clock edge (rising or falling
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

    // Reading assembly file to a string
    std::vector<std::string> asmVector;
    std::string asmString;
    asmString = file + ".asm";
    std::ifstream asmFile(asmString, std::ios_base::in);
    asmString.clear();
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

    // Create a program with the assembly and parse it
    Program prog(asmVector);
    prog.parse();

    if (verbose) {
        std::cout << "Parsed Code: (begins after leading NOPs)" << std::endl;
        prog.printCode();
        std::cout << std::endl << std::endl;
    }

    // Assemble the code
    prog.assemble();

    if (verbose) {
        std::cout << "Assembled Binary: (begins after leading NOPs)" << std::endl;
        prog.printBin();
        std::cout << std::endl << std::endl;
    }

    // Write the binary to a file
    if (writeOut) {
        asmString = file + ".bin";
        prog.writeFile(asmString);
        if (verbose) std::cout << "Assembly written!" << std::endl;
    }

    // Put the binary into a vector to put in RAM
    std::vector<unsigned short> binTemp;
    prog.seek(0);
    for (int i = 0; i < prog.size(); i++) {
        binTemp.push_back(prog.nextBin());
    }

    // Populate the zero page
    page0.init(binTemp);
    if (verbose) {
        std::cout << "Contents of Zero Page:" << std::endl;
        page0.dump();
    }

    // Run the program
    while (!processor.halt) {
        runClock();
        if (processor.processorState == 0 && gui) processor.dumpState();
        if (processor.processorState == 0 && slowDown) std::this_thread::sleep_for(std::chrono::milliseconds(timescale));
    }

    // Display processor messages (should be an output and a clock halt)
    if (!gui) {
        std::cout << std::endl << processor.message1 << std::endl;
        std::cout << processor.message2 << std::endl;
    }

    return 0;
}