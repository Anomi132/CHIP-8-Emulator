#include "string"

#include "cpu.hpp"
#include "MMU.hpp"

/*
	Get the next instruction based on the current position of the program counter
	Increments program counter to the next instruction
*/
uint16_t CPU::getNextInstruction()
{
	if (_programCounter >= 4096) // Max memorry
	{
		throw "programCounter outside memorry range (0-4095): " + std::to_string(_programCounter);
	}

	uint16_t opcode = _mmuRefrence.readMemorryAddress(_programCounter);
	_programCounter++;
	return opcode;
}

void CPU::executeOptCode(const uint16_t opcode)
{

}

/*
	Runs a single CPU Cycle
*/
void CPU::runClockCycle()
{
	uint16_t currentOptCode = getNextInstruction();
	executeOptCode(currentOptCode);
}

