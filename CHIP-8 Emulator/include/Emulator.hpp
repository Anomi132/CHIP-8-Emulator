#pragma once
#include <iostream>
#include <string>

#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_scancode.h>

#include "MMU.hpp"
#include "CPU.hpp"
#include "display.hpp"

class CPU;
class MMU;

/*
	The ClockManager makes sure that the emulator runs at the set clockspeed.
	We do this with a frametime accumulator that makes sure that we run at example 400Hz

	It can also perfroms N number of CPU cycles
*/

class Emulator
{
private:
	double _accumulator = 0.0;
	double _timerAccumulator = 0.0;

	double TIME_PER_CYCLE = 1.0 / 540.0; // For 540Hz
	double TIME_FOR_TIMER = 1.0 / 60.0; // 60Hz

	CPU _emulatorCPU;
	MMU _emulatorMMU;
	Display _emulatorDisplay;

	SDL_AudioStream* _audioStream = nullptr;
	int _waveCounter = 0;

	bool keypad[16] = { 0 };

	std::string _fileLoaded = "";

private:

	void playSound();

public:

	Emulator(SDL_Renderer** renderer);
	Emulator() {};
	~Emulator();

	void update(double deltaTime);

	bool getKeyStateById(uint8_t index);
	int getPressedKey();
	void updateKeyboard();

	void openFileWindow();
	void loadROM(std::string& file);
	void setFileName(std::string& file);
};