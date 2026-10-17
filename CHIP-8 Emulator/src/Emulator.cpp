#include <SDL3/SDL.h>

#include <string>

#include "Emulator.hpp"
#include "CPU.hpp"
#include "MMU.hpp"
#include "display.hpp"

Emulator::Emulator(SDL_Renderer** renderer)
{
	_emulatorMMU = MMU();
	_emulatorDisplay = Display(renderer, &_emulatorMMU);
	_emulatorCPU = CPU(&_emulatorMMU, &_emulatorDisplay);

	SDL_AudioSpec spec;
	spec.format = SDL_AUDIO_F32;
	spec.channels = 1;             
	spec.freq = 44100;

	_audioStream = SDL_OpenAudioDeviceStream(
		SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
		&spec,
		NULL,
		NULL
	);
	SDL_ResumeAudioStreamDevice(_audioStream);
}

Emulator::~Emulator()
{
	SDL_DestroyAudioStream(_audioStream);
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

/*
	Uses SDL3 to check what keys are down every frame. It uses scancodes to always represent the 4x4 keys starting with its upper 4 being 1234
*/
void Emulator::updateKeyboard()
{
	const bool* key_states = SDL_GetKeyboardState(NULL);

	static const SDL_Scancode key_map[16] = {
		SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4, // 0, 1, 2, 3
		SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_R, // 4, 5, 6, 7
		SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_F, // 8, 9, A, B
		SDL_SCANCODE_Z, SDL_SCANCODE_X, SDL_SCANCODE_C, SDL_SCANCODE_V  // C, D, E, F
	};

	for (int i = 0; i < 16; ++i)
	{
		keyboard[i] = key_states[key_map[i]] ? 1 : 0;
	}
}

/*
	Gives the state of a keyboard press based on index
*/
bool Emulator::getKeyStateById(uint8_t index)
{
	if (index >= 16)
	{
		throw "This key does not exist index: " + std::to_string(index);
	}

	return keyboard[index];
}