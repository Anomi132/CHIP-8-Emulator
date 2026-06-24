/*


*/
#pragma once	
#include <array>

class MMU
{
private:
	std::array<uint8_t, 4096> _memoryMap;

public:

	uint8_t readMemorryAddress(const uint16_t address);
	void writeToMemorryAddress(const uint16_t address, const uint8_t data);
};