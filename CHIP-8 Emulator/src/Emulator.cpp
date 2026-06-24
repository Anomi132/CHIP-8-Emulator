#include "Emulator.hpp"
#include "CPU.hpp"
#include "MMU.hpp"

CPU Emulator::emulatorCPU = CPU();
double Emulator::m_accumulator;

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
		throw "Test error message";
	}
}