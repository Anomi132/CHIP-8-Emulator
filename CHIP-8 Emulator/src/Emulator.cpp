#include "Emulator.hpp"

void Emulator::update(double deltaTime)
{
	m_accumulator += deltaTime;

	while (m_accumulator >= TIME_PER_CYCLE)
	{
		m_accumulator -= TIME_PER_CYCLE;
	}
}