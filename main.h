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
RamChip256 page0 (0);
RamChip256 page1(1);
Output portOut ((unsigned short)0x100);
RamChip256 page2 (2);
RamChip256 page254 (254);
RamChip256 page255 (255);

bool clk = false;