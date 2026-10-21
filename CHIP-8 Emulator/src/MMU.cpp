#include "MMU.hpp"

#include <array>
#include <string>
#include <cstdint>
#include <fstream>
#include <algorithm>
#include <iostream>

MMU::MMU()
{
	std::copy(CHIP8_FONTSET, CHIP8_FONTSET + 80, _memoryMap.begin());
}

/*
	Read memorry location at [address]
*/
uint8_t MMU::readMemorryAddress(const uint16_t address)
{
	if (address >= 4096) // Max memorry
	{
		throw std::out_of_range("Accessed address outside memorry range (0-4095): " + std::to_string(address));
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
		throw std::out_of_range("Written address outside memorry range (0-4095): " + std::to_string(address));
	}

	_memoryMap[address] = data;
}

/*
	Resets every memorry address to 0 and sets the fonts
*/
void MMU::resetMemorry()
{
	std::fill(_memoryMap.begin(), _memoryMap.end(), 0);
	std::copy(CHIP8_FONTSET, CHIP8_FONTSET + 80, _memoryMap.begin());
}

/*
	Loads bytes from file
*/
void MMU::loadFromFile(std::string& romLocation)
{
	std::ifstream file(romLocation, std::ios::binary | std::ios::ate);

	if (!file.is_open()) {
		throw std::runtime_error("Cant load from file on location: " + romLocation);
	}

	std::streampos size = file.tellg(); // Becuase of ate the index is at the end;

	if (size > (4096 - 0x200)) {
		throw std::runtime_error("ROM is too large to fit in CHIP-8 memory!");
	}

	file.seekg(0, std::ios::beg);

	file.read((char*)&_memoryMap[0x200], size);

	file.close();
}
