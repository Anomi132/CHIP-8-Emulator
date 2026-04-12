#include "Emulator.hpp"
#include "CPU.hpp"

CPU emulatorCPU = CPU();

/*
	Performs a arbitrary amount of CPU cycles to keep in sync with the FPS based on [TIME_PER_CYCLE]
*/
void Emulator::update(double deltaTime)
{
	m_accumulator += deltaTime;

	while (m_accumulator >= TIME_PER_CYCLE)
	{
		//emulatorCPU.runClockCycle();
		m_accumulator -= TIME_PER_CYCLE;
	}
}