#include "chipMemory.hpp"

uint8_t Memory::memoryMap[4096];

uint8_t Memory::readMemorryAddress(const uint16_t address)
{
	return memoryMap[address];
}

void Memory::writeToMemorryAddress(const uint16_t address, const uint8_t data)
{
	memoryMap[address] = data;
}