#include <SDL3/SDL.h>

#include <string>
#include <fstream>

#include "Emulator.hpp"
#include "CPU.hpp"
#include "MMU.hpp"
#include "display.hpp"

Emulator::Emulator(SDL_Renderer** renderer)
	: _emulatorMMU(),
	_emulatorDisplay(renderer, &_emulatorMMU),
	_emulatorCPU(&_emulatorMMU, &_emulatorDisplay, this)
{

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

	// TEMPORRAY ROM LOADING
	std::string location = "./roms/8-scrolling.ch8";
	this->loadROM(location);
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
	updateKeyboard();

	_accumulator += deltaTime;
	while (_accumulator >= TIME_PER_CYCLE)
	{
		_emulatorCPU.runClockCycle();
		_accumulator -= TIME_PER_CYCLE;
	}

	_timerAccumulator += deltaTime;
	while (_timerAccumulator >= TIME_FOR_TIMER)
	{
		if (_emulatorCPU.getDelayTimer() > 0)
		{
			_emulatorCPU.decrementDelayTimer();
		}

		if (_emulatorCPU.getSoundTimer() > 0)
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
			buffer[i] = (_waveCounter < 50) ? 0.01f : -0.01f;

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

	SDL_PutAudioStreamData(_audioStream, buffer, 735 * sizeof(float));
}

/*
	Uses SDL3 to check what keys are down every frame. It uses scancodes to always represent the 4x4 keys starting with its upper 4 being 1234
*/
void Emulator::updateKeyboard()
{
	const bool* key_states = SDL_GetKeyboardState(NULL);

	static const SDL_Scancode key_map[16] = {
		SDL_SCANCODE_X, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, // 0x0, 0x1, 0x2, 0x3
		SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_A, // 0x4, 0x5, 0x6, 0x7
		SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_Z, SDL_SCANCODE_C, // 0x8, 0x9, 0xA, 0xB
		SDL_SCANCODE_4, SDL_SCANCODE_R, SDL_SCANCODE_F, SDL_SCANCODE_V  // 0xC, 0xD, 0xE, 0xF
	};

	for (int i = 0; i < 16; ++i)
	{
		keypad[i] = key_states[key_map[i]] ? 1 : 0;
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

	return keypad[index];
}

/*
	Returns the index (0x0 to 0xF) of the first pressed key found.
	Returns -1 if no key is currently pressed.
*/
int Emulator::getPressedKey()
{
	for (int i = 0; i < 16; ++i)
	{
		if (keypad[i]) 
		{
			return i; 
		}
	}

	return -1; 
}

/*
	Resets states and loads the ROM at [romLocation]
*/
void Emulator::loadROM(std::string& romLocation)
{
	_emulatorCPU.resetState();
	_emulatorMMU.resetMemorry();
	_emulatorDisplay.clearScreen();

	_emulatorMMU.loadFromFile(romLocation);
}