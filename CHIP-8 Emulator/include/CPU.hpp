#pragma once

#include <array>
#include <stack>

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

class CPU
{
private:
	std::array <uint8_t, 16> _registers = { 0 };
	uint16_t _addressRegisterI;

	uint8_t _delayTimer = 0;
	uint8_t _soundTimer = 0;

	uint16_t _programCounter = 0x200; // Start of normal most CHIP-8 programs
	uint8_t _stackPointer = 0;

	std::stack<uint16_t> _stack;

	MMU _mmuRefrence;

private:
	uint16_t getNextInstruction();
	void executeOptCode(const uint16_t opcode);

public:
	CPU(MMU mmu) : _mmuRefrence(mmu) {}

	void runClockCycle();
};