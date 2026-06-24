#include "MMU.hpp"
#include <array>

std::array<uint8_t, 4096> MMU::memoryMap;

uint8_t MMU::readMemorryAddress(const uint16_t address)
{
	return memoryMap[address];
}

void MMU::writeToMemorryAddress(const uint16_t address, const uint8_t data)
{
	memoryMap[address] = data;
}