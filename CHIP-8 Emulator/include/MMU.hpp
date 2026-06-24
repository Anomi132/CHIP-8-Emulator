/*


*/
#pragma once	
#include <vector>

class MMU
{
private:
	static std::array<uint8_t, 4096> memoryMap;

public:

	static uint8_t readMemorryAddress(const uint16_t address);
	static void writeToMemorryAddress(const uint16_t address, const uint8_t data);
};