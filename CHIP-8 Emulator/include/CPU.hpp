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
	std::array <uint8_t, 16> registers = { 0 };
	uint16_t addressRegisterI;

	uint8_t delayTimer = 0;
	uint8_t soundTimer = 0;

	uint16_t programCounter = 0x200; // Start of normal most CHIP-8 programs
	uint8_t stackPointer = 0;

	std::stack<uint16_t> stack;

private:
	 inline uint16_t getNextInstruction();
	 void decodeAndExecuteInstruction(const uint16_t opcode);

public:
	 void runClockCycle();
};