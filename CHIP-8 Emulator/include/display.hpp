#pragma once
#include "SDL3/SDL.h"
#include "MMU.hpp"

#define DISPLAY_SCALE_FACTOR 22
#define DEFAULT_WIDTH 64
#define DEFAULT_HEIGHT 32	

class Display
{
private:
	SDL_Renderer* _displayRendererRefrence = nullptr;
	MMU* _mmuRefrence = nullptr;

	bool _displayArray[DEFAULT_HEIGHT][DEFAULT_WIDTH] = { 0 }; // Array that holds the display state so we can flip

public:
	Display(SDL_Renderer** renderer, MMU* mmu);
	Display() {};


	void draw();
	void clearScreen();
	void drawSprite(uint16_t x, uint16_t y, uint16_t address, uint16_t numberOfBytes);

};