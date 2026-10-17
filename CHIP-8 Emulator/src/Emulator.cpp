#include <SDL3/SDL.h>

#include "Emulator.hpp"
#include "CPU.hpp"
#include "MMU.hpp"
#include "display.hpp"

Emulator::Emulator(SDL_Renderer** renderer)
{
	_emulatorMMU = MMU();
	_emulatorDisplay = Display(renderer, &_emulatorMMU);
	_emulatorCPU = CPU(&_emulatorMMU, &_emulatorDisplay);

	_audioStream = SDL_OpenAudioDeviceStream(
		SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
		&spec,
		NULL,
		NULL
	);
	SDL_ResumeAudioStreamDevice(_audioStream);
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
	}

	_timerAccumulator += deltaTime;
	while (_timerAccumulator >= TIME_FOR_TIMER)
	{
		if (_emulatorCPU.getDelayTimer() >= 0)
		{
			_emulatorCPU.decrementDelayTimer();
		}

		if (_emulatorCPU.getSoundTimer() >= 0)
		{
			playSound();
			_emulatorCPU.decrementSoundTimer();
		}

		_timerAccumulator -= TIME_FOR_TIMER;
	}
}


/*
	Uses SDL3 to play a simple beep sound
*/
void Emulator::playSound()
{
	float buffer[735]; // 44100 per second for windows default / 60 frames per second for timer = 735 samples because timer decrements once a second

	if (_emulatorCPU.getSoundTimer() > 0)
	{
		for (int i = 0; i < 735; ++i)
		{
			// 440 Hz square wave: 50 high, 50 low
			buffer[i] = (_waveCounter < 50) ? 0.2f : -0.2f;

			_waveCounter++;
			if (_waveCounter >= 100) {
				_waveCounter = 0;
			}
		}
	}
	else
	{
		for (int i = 0; i < 735; ++i) {
			buffer[i] = 0.0f;
		}
	}
}