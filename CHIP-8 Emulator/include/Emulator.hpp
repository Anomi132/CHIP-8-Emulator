#pragma once
#include <iostream>

#include "CPU.hpp"

/*
	The ClockManager makes sure that the emulator runs at the set clockspeed.
	We do this with a frametime accumulator that makes sure that we run at example 400Hz

	It can also perfroms N number of CPU cycles
*/

static class Emulator
{
private:
	static double m_accumulator;
	static constexpr double TIME_PER_CYCLE = 1.0 / 400; // For 400Hz

	static CPU emulatorCPU;

public:

	static void update(double deltaTime);
};