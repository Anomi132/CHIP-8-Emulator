#include <SDL3/SDL.h>
#include <iostream>

/*
	The ClockManager makes sure that the emulator runs at the set clockspeed.
	We do this with a frametime accumulator that makes sure that we run at example 400Hz

	It can also perfroms N number of CPU cycles
*/

static class Emulator
{
private:
	static inline double m_accumulator = 0.0;
	static constexpr double TIME_PER_CYCLE = 1.0 / 400; // For 400Hz

public:

	static void update(double deltaTime);
};