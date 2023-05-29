#include "processor.h"

Processor::Processor(unsigned short pc) {
    programCounter = pc;
}

Processor::Processor() {
    programCounter = 0;
}

void Processor::raiseInterrupt() {
    interrupt = true;
}

void Processor::run(bool clock) {
    if (clock == lastClock) return;
    if (halt && !interrupt) return;
    else halt = false;

    lastClock = clock;

    unsigned short operation = instructionRegister;
    unsigned short microcode1 = binToMicro1.at((unsigned short)(instructionRegister >> 11));
    unsigned short microcode2 = binToMicro2.at((unsigned short)(instructionRegister >> 11));

    unsigned short operand = instructionRegister & 255;
    unsigned char reg = (unsigned char)((instructionRegister & 0b11100000000) >> 8);

    switch (processorState) {
    case 0:
        if (clock) {
            busAddressLatch = programCounter; // PA
            readRequest = true;
        }
        else {
            instructionRegister = busInputRegister; // IL
        }
        break;
    case 1:
        if (clock) {
            programCounter++; // PI
        }
        else {
            busAddressLatch = busAddressRegister;
        }
        break;
    case 2:
        if (clock) {
            // rising edge
            if (microcode1 & 2) {
                busAddressLatch = busAddressRegister;
                readRequest = true;
                busOut = true;
            }
            if (microcode1 & 8)    addrOut = true;
            if (microcode1 & 512)  accOut = true;
            if (microcode1 & 2048) regOut = true;
            if (microcode1 & 8192) opLow = true;
        }
        else {
            // falling edge
            if (microcode1 & 1)    busIn = true;
            if (microcode1 & 4)    mathIn = true;
            if (microcode1 & 16)   accInLow = true;
            if (microcode1 & 32)   pcIn = true;
            if (microcode1 & 64)   arLowIn = true;
            if (microcode1 & 128)  arIn = true;
            if (microcode1 & 256)  accIn = true;
            if (microcode1 & 1024) regIn = true;
            if (microcode1 & 4096) addrHigh = true;
        }
        break;
    case 3:
        if (clock) {
            // rising edge
            if (microcode2 & 2) { carryFlag = false; zeroFlag = false; }
            if (microcode2 & 16) interrupt = false;
            if (microcode2 & 256) cRollLine = true;
            if (microcode2 & 512) cInLine = true;
            if (microcode2 & 1024) rightShift = true;
            if (microcode2 & 2048) leftShift = true;
            if (microcode2 & 4096) invBLine = true;
            if (microcode2 & 8192) invALine = true;
            if (microcode2 & 16384) fcLine = true;
            if (microcode2 & 32768) orLine = true;
        }
        else {
            // falling edge
            if (microcode2 & 1) actionStep(operand, reg); // AS
            else if (microcode2 & 32 && interrupt) actionStep(operand, reg);  // EI
            else if (microcode2 & 64 && zeroFlag) actionStep(operand, reg);   // EZ
            else if (microcode2 & 128 && carryFlag) actionStep(operand, reg); // EC

            if (microcode2 & 8) {
                halt = true;
                std::ostringstream ss;
                ss << std::endl << " Clock Halted! Accumulator Value: " << accumulator
                    << " | 0x" << int_to_hex(accumulator, 4)
                    << " | 0b" << int_to_bin(accumulator, 16);
                message2 = ss.str();
            } // CH
            opLow = false;
            addrHigh = false;
            regOut = false;
            regIn = false;
            accOut = false;
            accIn = false;
            arIn = false;
            arLowIn = false;
            pcIn = false;
            accInLow = false;
            addrOut = false;
            mathIn = false;
            busOut = false;
            busIn = false;

            aluEnable = false;
            carryIn = false;

            orLine = false;
            fcLine = false;
            invALine = false;
            invBLine = false;
            leftShift = false;
            rightShift = false;
            cInLine = false;
            cRollLine = false;

            numInstructionsRun++;
        }
        break;
    }

    if (!clock) processorState = (processorState + 1) % 4;
    return;
}

void Processor::actionStep(unsigned short operand, unsigned char reg) {
    mathInput = 0;

    if (opLow) {
        mathInput = mathInput | operand;
    }
    if (regOut) {
        mathInput = mathInput | ((reg > 0) ? gpr[reg] : 0);
    }
    if (accOut) {
        mathInput = mathInput | accumulator;
    }
    if (addrOut) {
        mathInput = mathInput | busAddressRegister;
    }
    if (busOut) {
        mathInput = mathInput | busInputRegister;
    }

    if (mathIn) { // This only runs if MI is on, this is because flags get clobbered each time this runs
        inputA = (invALine) ? (~accumulator) : (accumulator);
        inputB = (invBLine) ? (~mathInput) : (mathInput);

        if (cInLine) carryIn = true;
        else if (cRollLine) carryIn = carryFlag;
        if (fcLine) carryIn = false; // Carry-in already accounted for when FC is active, so don't double-activate it!

        if (orLine && fcLine) result = ~(inputA | inputB); // OR + FC = NOR
        else if (orLine) result = inputA | inputB;         // OR +    = OR
        else if (fcLine) result = ~(inputA ^ inputB);      //    + FC = XNOR
        else result = inputA + inputB;                     //    +    = Addition

        result = result + carryIn;

        if (leftShift) result = result << 1;
        if (rightShift) result = result >> 1;

        carryFlag = false;
        zeroFlag = false;
        if (result >> 16) carryFlag = true;
        if ((result & 65535) == 0) zeroFlag = true;

        accumulator = (unsigned short)result;
    }

    if (addrHigh) {

        busAddressRegister = (busAddressRegister & 255);

        busAddressRegister += ((mathInput & 255) << 8);
    }
    if (regIn) {
        if (reg > 0) {
            gpr[reg] = mathInput;
        }
    }
    if (accIn) {
        accumulator = mathInput;
        if (opLow) accumulator = accumulator << 8;
    }
    if (arIn) {
        busAddressRegister = mathInput;
    }
    if (arLowIn) {

        busAddressRegister = (busAddressRegister >> 8) << 8;
        
        busAddressRegister = busAddressRegister + (mathInput & 255);
    }
    if (pcIn) { programCounter = mathInput; }
    if (accInLow) {
        accumulator = (accumulator & 0b1111111100000000) + (mathInput % 256);
    }

    if (busIn) {
        busAddressLatch = busAddressRegister;
        busOutputRegister = mathInput;
        writeRequest = true;
    }
}

void Processor::dumpState() {
    std::ostringstream acc;
    acc << "    " << accumulator << " | 0x" << int_to_hex(accumulator, 4) << " | 0b" << int_to_bin(accumulator, 16) << std::endl;

    std::ostringstream gpr1, gpr2, gpr3, gpr4, gpr5, gpr6, gpr7;
    gpr1 << "    1 - " << gpr[1] << std::endl;
    gpr2 << "    2 - " << gpr[2] << std::endl;
    gpr3 << "    3 - " << gpr[3] << std::endl;
    gpr4 << "    4 - " << gpr[4] << std::endl;
    gpr5 << "    5 - " << gpr[5] << std::endl;
    gpr6 << "    6 - " << gpr[6] << std::endl;
    gpr7 << "    7 - " << gpr[7] << std::endl;

    std::ostringstream ins;
    ins << "Instruction: " << binToOp.at(instructionRegister >> 11);

    std::ostringstream bar;
    bar << busAddressRegister;

    std::ostringstream pc;
    pc << "    " << programCounter << std::endl;

    document = dbox({
        vbox({
            text(" ========================= PROCESSOR STATE DUMP ========================= "),
            text("Accumulator Value: "),
            text(acc.str()),
            text(""),
            text("GPRs: "),
            text(gpr1.str()),
            text(gpr2.str()),
            text(gpr3.str()),
            text(gpr4.str()),
            text(gpr5.str()),
            text(gpr6.str()),
            text(gpr7.str()),
            text(""),
            text("Program Counter Value: "),
            text(pc.str()),
            text(""),
            text(ins.str()),
            text(""),
            text("Bus Address Register: "),
            text(bar.str()),
            text(""),
            text(message1),
            text(message2),
            text("")
        }) | border
    });

    std::string reset_position;
    screen = Screen::Create(
        Dimension::Fit(document),
        Dimension::Fit(document)
    );
    ftxui::Render(screen, document);

    reset_position = screen.ResetPosition();

    std::cout << reset_position;
    screen.Print();

}