#include "MMU.hpp"

#include <array>
#include <string>

/*
	Read memorry location at [address]
*/
uint8_t MMU::readMemorryAddress(const uint16_t address)
{
	if (address >= 4096) // Max memorry
	{
		throw "Accessed address outside memorry range (0-4095): " + std::to_string(address);
	}

	return _memoryMap[address];
}

/*
	Write [data] to memorry address at [address]
*/
void MMU::writeToMemorryAddress(const uint16_t address, const uint8_t data)
{
	if (address >= 4096) // Max memorry
	{
		throw "Written address outside memorry range (0-4095): " + std::to_string(address);
	}

	_memoryMap[address] = data;
}