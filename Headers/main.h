#pragma once

#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

#include "io.h"
#include "ramchip.h"
#include "processor.h"
#include "assembler.h"


Processor processor;
RamChip256 lowPages[8] = { RamChip256(0, true), RamChip256(1, true),
                           RamChip256(2, true), RamChip256(3, true),
                           RamChip256(4), RamChip256(5),
                           RamChip256(6), RamChip256(7) };
Output portOut ((unsigned short)0x8000);
Input portIn((unsigned short)0x8001);
RamChip256 page254 (254);
RamChip256 page255 (255);

bool clk = false;