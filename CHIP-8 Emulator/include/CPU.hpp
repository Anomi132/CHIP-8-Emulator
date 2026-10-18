#pragma once

#include <array>
#include <stack>

#include "MMU.hpp"
#include "display.hpp"
#include "emulator.hpp"

enum registers
{
	V0,
	V1,
	V2,
	V3,
	V4,
	V5,
	V6,
	V7,
	V8,
	V9,
	VA,
	VB,
	VC,
	VD,
	VE,
	VF,
};

/*
	Representation of the CHIP-8 CPU. 
	Functions to decode and execute opcodes
	Opcodes and architecture obtained from: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
*/

struct cpuState
{
	std::array <uint8_t, 16> registers = { 0 };
	uint16_t addressRegisterI = 0x0;

	uint8_t delayTimer = 0;
	uint8_t soundTimer = 0;

	uint16_t programCounter = 0x200; // Start of normal most CHIP-8 programs
	uint8_t stackPointer = 0;

	std::stack<uint16_t> _stack;

	MMU* mmuRefrence = nullptr;
	Display* displayRefrence = nullptr;

	Emulator* emulatorRefrence = nullptr;
};

class CPU
{
private:
	cpuState state;

	uint16_t getNextInstruction();
	void executeOptCode(const uint16_t opcode);

public:
	CPU(MMU* mmu, Display* display, Emulator* emulator);
	CPU() {}

	void runClockCycle();

	void decrementDelayTimer();
	void decrementSoundTimer();

	uint8_t getDelayTimer();
	uint8_t getSoundTimer();

	void resetState();
};