// DJ-CPU-Emulator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "main.h"

// Output the parsed code, assembled binary, and contents of zero page of RAM
const bool verbose = false;

// Write assembly file
const bool writeOut = false;

// Use GUI to display processor internals
const bool gui = true;

// Slow down execution
const bool slowDown = false;

// How much to wait between instructions
const int timescale = 10;

// Dump zero page after execution ends
const bool dumpZeroPage = true;
const bool dumpOnePage = false;
const bool dumpTwoPage = false;
const bool dumpThreePage = false;
const bool dumpFourPage = true;
const bool dumpFivePage = false;
const bool dumpSixPage = false;
const bool dumpSevenPage = false;

// Filename to read assembly from
std::string asmFile = "subroutine.asm";

// Filename to write binary to
std::string binFile = "rom.bin";

// Bus write event
void writeBus(unsigned short address, unsigned short value)
{
    std::string str;
    for (int i = 0; i < 8; i++) {
        str = lowPages[i].busWriteRequested(address, value);
        if (str != "") processor.message2 = str;
    }
    page254.busWriteRequested(address, value);
    page255.busWriteRequested(address, value);

    str = portOut.busWriteRequested(address, value);
    if (str != "") processor.message1 = str;
}

// Bus read event
unsigned short readBus(unsigned short address)
{
    unsigned short port = 0;
    unsigned short bus = 0;
    for (int i = 0; i < 8; i++) bus = bus | lowPages[i].busReadRequested(address);
    port = portIn.busReadRequested(address);
    if (port) processor.raiseInterrupt();
    bus = bus | port;
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
    std::ifstream file(asmFile, std::ios_base::in);
    int i = 0;
    if (file.is_open()) {
        while (std::getline(file, asmString)) {
            asmVector.push_back(asmString);
            if (verbose) std::cout << asmString << std::endl;
        }
        file.close();
    }
    else {
        asmFile = "Assembly/" + asmFile;
        file = std::ifstream(asmFile, std::ios_base::in);
        if (file.is_open()) {
            while (std::getline(file, asmString)) {
                asmVector.push_back(asmString);
                if (verbose) std::cout << asmString << std::endl;
            }
            file.close();
        }
        else {
            if (verbose) std::cout << "Failed to read assembly file!";
            return 1;
        }
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
        prog.writeFile(binFile);
        if (verbose) std::cout << "Assembly written!" << std::endl;
    }

    // Put the binary into a vector to put in RAM
    std::vector<unsigned short> binTemp;
    prog.seek(0);
    for (int i = 0; i < prog.size(); i++) {
        binTemp.push_back(prog.nextBin());
    }
    if (binTemp.size() > 2048) {
        std::cout << "Assembled binary too large to fit in first 8 pages of RAM!" << std::endl;
        return 2;
    }

    // Populate the low pages
    int page = 0;
    int s = binTemp.size();
    while (s > 0) {
        lowPages[page].init(binTemp);
        if (s >= 256) binTemp.erase(binTemp.begin(), binTemp.begin() + 256);
        else binTemp.clear();
        s = binTemp.size();
        page++;
    }

    // Dump the zero page before execution
    if (verbose) {
        std::cout << "Contents of Zero Page:" << std::endl;
        lowPages[0].dump();
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
    else processor.dumpState();

    // Dump memory after execution
    std::cout << std::endl;
    if (dumpZeroPage) {
        std::cout << "Zero Page" << std::endl;
        lowPages[0].dump();
    }
    if (dumpOnePage) {
        std::cout << "One Page" << std::endl;
        lowPages[1].dump();
    }
    if (dumpTwoPage) {
        std::cout << "Two Page" << std::endl;
        lowPages[2].dump();
    }
    if (dumpThreePage) {
        std::cout << "Three Page" << std::endl;
        lowPages[3].dump();
    }
    if (dumpFourPage) {
        std::cout << "Four Page" << std::endl;
        lowPages[4].dump();
    }
    if (dumpFivePage) {
        std::cout << "Five Page" << std::endl;
        lowPages[5].dump();
    }
    if (dumpSixPage) {
        std::cout << "Six Page" << std::endl;
        lowPages[6].dump();
    }
    if (dumpSevenPage) {
        std::cout << "Seven Page" << std::endl;
        lowPages[7].dump();
    }

    return 0;
}