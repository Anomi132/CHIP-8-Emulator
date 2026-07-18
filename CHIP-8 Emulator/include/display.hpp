#pragma once
#include "SDL3/SDL.h"

#define DISPLAY_SCALE_FACTOR 20
#define DEFAULT_WIDTH 64
#define DEFAULT_HEIGHT 32	

class Display
{
private:
	SDL_Renderer* _displayRenderer = nullptr;

	bool _displayArray[DEFAULT_HEIGHT][DEFAULT_WIDTH] = { 0 }; // Array that holds the display state so we can flip

public:
	Display(SDL_Renderer** renderer);
	Display() {};

	void draw();
	void clearScreen();

};