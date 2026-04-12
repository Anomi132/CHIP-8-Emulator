/*


*/
#pragma once	
#include <vector>

class Memory
{
private:
	static uint8_t memoryMap[4096];

public:

	static uint8_t readMemorryAddress(const uint16_t address);
	static void writeToMemorryAddress(const uint16_t address, const uint8_t data);
};