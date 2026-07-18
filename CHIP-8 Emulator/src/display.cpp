#include "display.hpp"


Display::Display(SDL_Renderer** renderer)
{
	_displayRenderer = *renderer;
}

/*
    Clears screen
*/
void Display::clearScreen()
{
    SDL_RenderClear(_displayRenderer);
}

void Display::draw()
{
    SDL_FRect r;
    r.x = 50;
    r.y = 50;
    r.w = 50;
    r.h = 50;

    SDL_SetRenderDrawColor(_displayRenderer, 0, 0, 255, 255);

    SDL_RenderFillRect(_displayRenderer, &r);

    SDL_RenderPresent(_displayRenderer);
}