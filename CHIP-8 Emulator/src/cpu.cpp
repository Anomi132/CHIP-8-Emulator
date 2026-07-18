#include <string>
#include <iostream>

#include "cpu.hpp"
#include "MMU.hpp"

/*
	Get the next instruction based on the current position of the program counter
	Increments program counter to the next instruction
*/
opcode CPU::getNextInstruction()
{
	if (_programCounter >= 4096) // Max memorry
	{
		throw "programCounter outside memorry range (0-4095): " + std::to_string(_programCounter);
	}

	uint16_t opcodeUpper = _mmuRefrence->readMemorryAddress(_programCounter);
	uint16_t opcodeLower = _mmuRefrence->readMemorryAddress(_programCounter + 1);
	uint16_t fullOpcode = (opcodeUpper << 8) | opcodeLower;

	_programCounter += 2;
	return static_cast<opcode>(fullOpcode);
}

void CPU::executeOptCode(const opcode opcode)
{

}

/*
	Runs a single CPU Cycle
*/
void CPU::runClockCycle()
{
	opcode currentOptCode = getNextInstruction();
	executeOptCode(currentOptCode);
}

