#include "Emulator.hpp"
#include "CPU.hpp"
#include "MMU.hpp"

Emulator::Emulator()
{
	_emulatorMMU = MMU();
	_emulatorCPU = CPU(_emulatorMMU);
}

/*
	Performs a arbitrary amount of CPU cycles to keep in sync with the FPS based on [TIME_PER_CYCLE]
*/
void Emulator::update(double deltaTime)
{
	_accumulator += deltaTime;

	while (_accumulator >= TIME_PER_CYCLE)
	{
		//emulatorCPU.runClockCycle();
		_accumulator -= TIME_PER_CYCLE;
		throw "Test error message";
	}
}