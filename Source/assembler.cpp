#include "assembler.h"

void Instruction::setOpcode(std::string opcode) {
    this->opcode = opcode;
}

void Instruction::setReg(unsigned char reg) {
    this->reg = reg;
}

void Instruction::setOperand(unsigned short operand) {
    this->operand = operand;
}

unsigned short Instruction::getBin() {
    if (stale) encode();
    return binary;
}

void Instruction::encode() {
    binary = opToBin.at(opcode) << 3;
    binary = (binary | (reg & 0b111)) << 8;
    binary = binary | (operand & 255);
    stale = false;
}

std::string Instruction::print() {
    std::string ret = opcode;
    ret.append(" ");
    ret.append(std::to_string((int)reg));
    ret.append(" 0x");
    ret.append(int_to_hex(operand, 4));
    return ret;
}

Program::Program(std::vector<std::string> code)
{
    this->code = code;
    currentInstruction = 0;
    parsed = false;
    assembled = false;
}

Program::Program()
{
    this->code = std::vector<std::string>();
    currentInstruction = 0;
    parsed = false;
    assembled = false;
}

void Program::setCode(std::vector<std::string> code)
{
    this->code = code;
    this->currentSize = 0;
    parsed = false;
    assembled = false;
}
void Program::setCode(std::string code)
{
    this->code.clear();
    this->code.push_back(code);
    this->currentSize = 0;
    parsed = false;
    assembled = false;
}

void Program::appendCode(std::vector<std::string> code)
{
    this->code.insert(this->code.end(), code.begin(), code.end());
    this->currentSize = 0;
    parsed = false;
    assembled = false;
}
void Program::appendCode(std::string code)
{
    this->code.push_back(code);
    this->currentSize = 0;
    parsed = false;
    assembled = false;
}

void Program::seek(unsigned short line)
{
    currentInstruction = line;
    if (currentInstruction >= instructions.size()) currentInstruction = 0;
}

unsigned short Program::getBin(unsigned short line)
{
    seek(line);
    unsigned short ret = instructions.at(currentInstruction).getBin();
    currentInstruction++;
    return ret;
}

unsigned short Program::nextBin()
{
    unsigned short ret = instructions.at(currentInstruction).getBin();
    currentInstruction++;
    return ret;
}

// Convert from parsed code to assembly
void Program::assemble()
{
    if (!parsed) parse();

    binary.clear();

    for (int i = 0; i < currentSize; i++) {
        binary.push_back(instructions.at(i).getBin());
    }

    assembled = true;
}

// Convert from read code to parsed code, handling special macros
void Program::parse()
{
    currentInstruction = 0;
    unsigned short iter = (unsigned short)(code.size());
    std::stringstream stream;
    std::vector<std::string> tokens;
    std::string tempString;

    std::string opc = "NOP";
    unsigned char reg = 0;
    unsigned short opr = 0;

    for (int i = 0; i < iter; i++) {
        tokens.clear();
        stream = std::stringstream(code.at(i));

        while (std::getline(stream, tempString, ' ')) {
            if (tempString != "") tokens.push_back(tempString);
        }

        if (tokens.size() == 0) continue;

        if (opToBin.count(tokens.at(0))) {
            currentInstruction++;
        }
        else if (tokens.at(0) == "IMV") {
            currentInstruction++;
            currentInstruction++;
        }
        else if (tokens.at(0) == "IMT") {
            currentInstruction++;
            currentInstruction++;
        }
        else if (tokens.at(0) == "IAT") {
            currentInstruction++;
            currentInstruction++;
        }
        else if (tokens.at(0) == "MEM") {
            unsigned short num = (unsigned short)(std::stoi(tokens.at(1).substr(1)));
            if (num > currentInstruction) {
                currentInstruction = num;
            }
        }
        if (tokens.at(0).at(0) == '.') {
            tags.insert({ tokens.at(0), currentInstruction });
            tagsInverse.insert({ currentInstruction, tokens.at(0) });
        }
    }
    currentInstruction = 0;
    for (int i = 0; i < iter; i++) {
        bool comment = false;
        opc = "NOP";
        reg = 0;
        opr = 0;
        tokens.clear();
        stream = std::stringstream(code.at(i));

        while (std::getline(stream, tempString, ' ')) {
            if (tempString.substr(0,2) == "//") comment = true;
            if (!comment && tempString.size() > 1) {
                tokens.push_back(tempString);
            }
        }

        if (tokens.size() == 0) continue;

        if (opToBin.count(tokens.at(0))) {
            opc = tokens.at(0);
            opr = 0;
            reg = 0;
            if (tokens.size() > 1) {
                switch (tokens.at(1).at(0)) {
                case '#':
                    opr = std::stoi(tokens.at(1).substr(1));
                    reg = 0;
                    break;
                case 'R':
                    reg = std::stoi(tokens.at(1).substr(1));
                    opr = 0;
                    break;
                default:
                    reg = 0;
                    opr = 0;
                }
            }
            else if (noOperand.count(tokens.at(0)) == 0) std::cout << "No operand at " << i << " (assuming 0)" << std::endl;
            instructions.push_back(Instruction(opc, reg, opr));
            currentInstruction++;
        }
        else if (tokens.at(0) == "IMV") {
            if (tokens.size() > 1) {
                std::string numS = tokens.at(1);
                if (numS.at(0) == '#') numS = numS.substr(1);

                opc = "IMH";
                opr = ((unsigned short)(std::stoi(numS)));
                opr = opr >> 8;
                reg = 0;
                instructions.push_back(Instruction(opc, reg, opr));
                currentInstruction++;

                opc = "IML";
                opr = ((unsigned short)(std::stoi(numS)) % 256);
                reg = 0;
                instructions.push_back(Instruction(opc, reg, opr));
                currentInstruction++;
            }
            else std::cout << "No operand at " << i << " (skipping)" << std::endl;
        }
        else if (tokens.at(0) == "IMT") {
            if (tokens.size() > 1) {
                opc = "IMH";
                opr = (unsigned short)(tags.at(tokens.at(1))) >> 8;
                reg = 0;
                instructions.push_back(Instruction(opc, reg, opr));
                currentInstruction++;

                opc = "IML";
                opr = (unsigned short)(tags.at(tokens.at(1))) & 255;
                reg = 0;
                instructions.push_back(Instruction(opc, reg, opr));
                currentInstruction++;
            }
            else std::cout << "No operand at " << i << " (skipping)" << std::endl;
        }
        else if (tokens.at(0) == "IAT") {
            if (tokens.size() > 1) {
                opc = "IAH";
                opr = (unsigned short)(tags.at(tokens.at(1))) >> 8;
                reg = 0;
                instructions.push_back(Instruction(opc, reg, opr));
                currentInstruction++;

                opc = "IAL";
                opr = (unsigned short)(tags.at(tokens.at(1))) & 255;
                reg = 0;
                instructions.push_back(Instruction(opc, reg, opr));
                currentInstruction++;
            }
            else std::cout << "No operand at " << i << " (skipping)" << std::endl;
        }
        else if (tokens.at(0) == "MEM") {
            if (tokens.size() > 1) {
                std::string numS = tokens.at(1);
                if (numS.at(0) == '#') numS = numS.substr(1);
                unsigned short num = (unsigned short)(std::stoi(numS));
                if (num > currentInstruction) {
                    for (int j = 0; j < num - currentInstruction; j++) {
                        instructions.push_back(Instruction("NOP", 0, 0));
                    }
                    currentInstruction = num;
                }
            }
            else std::cout << "No operand at " << i << " (skipping)" << std::endl;
        }
    }
    currentSize = (unsigned short)(instructions.size());
    parsed = true;
}

// Print out parsed code, starting at first useful instruction
void Program::printCode()
{
    bool intro = true;
    for (int i = 0; i < currentSize; i++) {
        if (intro && (instructions.at(i).opcode != "NOP")) intro = false;
        if (!intro) std::cout << "0x" << int_to_hex(i, 4) << ' ' << instructions.at(i).print() << std::endl;
    }
}

// Print out assembled binary, starting at first useful instruction
void Program::printBin()
{
    bool intro = true;
    for (int i = 0; i < currentSize; i++) {
        if (intro && (binary.at(i) >> 11 != 0)) intro = false;
        if (!intro) {
            std::string ins = "";
            ins.append(int_to_bin(binary.at(i) >> 11, 5) + " ");
            ins.append(int_to_bin((binary.at(i) >> 8) & 0b111, 3) + " ");
            ins.append(int_to_bin(binary.at(i) & 0b11111111, 8));
            std::cout << "0x" << int_to_hex(i, 4) << " | " << int_to_bin(binary.at(i), 16) << " | " << ins << std::endl;
        }
    }
}

// Write binary to file
void Program::writeFile(std::string fname)
{
    std::ofstream file;
    file.open(fname, std::ios::binary);
    for (int i = 0; i < binary.size(); i++) {
        file.write(reinterpret_cast<char*>(&binary.at(i)), sizeof(binary.at(i)));
    }
    file.close();
}