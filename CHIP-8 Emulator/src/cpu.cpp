#include <string>
#include <iostream>
#include <cstdlib>
#include <time.h>

#include "cpu.hpp"
#include "MMU.hpp"

CPU::CPU(MMU* mmu, Display* display)
{
	state.mmuRefrence = mmu;
	state.displayRefrence = display;
}

/*
	Get the next instruction based on the current position of the program counter
	Increments program counter to the next instruction
*/
uint16_t CPU::getNextInstruction()
{
	if (state.programCounter >= 4096) // Max memorry
	{
		throw "programCounter outside memorry range (0-4095): " + std::to_string(state.programCounter);
	}

	uint16_t opcodeUpper = state.mmuRefrence->readMemorryAddress(state.programCounter);
	uint16_t opcodeLower = state.mmuRefrence->readMemorryAddress(state.programCounter + 1);
	uint16_t fullOpcode = (opcodeUpper << 8) | opcodeLower;

	state.programCounter += 2;
	return static_cast<uint16_t>(fullOpcode);
}

/*
	TIMERS
*/
void CPU::decrementDelayTimer()
{
	if (state.delayTimer <= 0) { return; }

	state.delayTimer -= 1;
}

void CPU::decrementSoundTimer()
{
	if (state.soundTimer <= 0) { return; }

	state.soundTimer -= 1;
}

uint8_t CPU::getDelayTimer()
{
	return state.delayTimer;
}

uint8_t CPU::getSoundTimer()
{
	return state.soundTimer;
}

/*
	Runs a single CPU Cycle
*/
void CPU::runClockCycle()
{
	uint16_t currentOptCode = getNextInstruction();
	executeOptCode(currentOptCode);
}
