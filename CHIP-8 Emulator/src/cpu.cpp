#include "cpu.hpp"
#include "MMU.hpp"

#define MAX_MEMORY 4096

inline uint16_t CPU::getNextInstruction()
{
	if (programCounter > MAX_MEMORY)
	{
		 
	}

	uint16_t opcode = MMU::readMemorryAddress(programCounter);
	programCounter++;
	return opcode;
}