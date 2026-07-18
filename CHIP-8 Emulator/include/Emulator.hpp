#pragma once
#include <iostream>

#include "CPU.hpp"
#include "MMU.hpp"
#include "display.hpp"

/*
	The ClockManager makes sure that the emulator runs at the set clockspeed.
	We do this with a frametime accumulator that makes sure that we run at example 400Hz

	It can also perfroms N number of CPU cycles
*/

class Emulator
{
private:
	double _accumulator = 0.0;
	double TIME_PER_CYCLE = 1.0 / 400; // For 400Hz

	CPU _emulatorCPU;
	MMU _emulatorMMU;
	Display _emulatorDisplay;

public:

	Emulator(SDL_Renderer** renderer);

	void update(double deltaTime);
};