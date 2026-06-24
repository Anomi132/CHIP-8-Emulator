#pragma once
#include <iostream>

#include "CPU.hpp"
#include "MMU.hpp"

/*
	The ClockManager makes sure that the emulator runs at the set clockspeed.
	We do this with a frametime accumulator that makes sure that we run at example 400Hz

	It can also perfroms N number of CPU cycles
*/

class Emulator
{
private:
	double _accumulator;
	double TIME_PER_CYCLE = 1.0 / 400; // For 400Hz

	CPU _emulatorCPU;
	MMU _emulatorMMU;


public:

	Emulator();

	void update(double deltaTime);
};