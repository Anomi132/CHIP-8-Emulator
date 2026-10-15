#include "display.hpp"


Display::Display(SDL_Renderer** renderer, MMU* mmu)
{
	_displayRendererRefrence = *renderer;
    _mmuRefrence = mmu;
}

/*
    Clears screen
*/
void Display::clearScreen()
{
    SDL_SetRenderDrawColor(_displayRendererRefrence, 0, 0, 0, 255);
    SDL_RenderClear(_displayRendererRefrence);

    memset(_displayArray, 0, sizeof _displayArray);
}


/*
    Draws the current display state based on [_displayArray]
*/
void Display::draw()
{
    SDL_FRect r;
    r.w = 1 * DISPLAY_SCALE_FACTOR;
    r.h = 1 * DISPLAY_SCALE_FACTOR;


    SDL_SetRenderDrawColor(_displayRendererRefrence, 0, 0, 0, 255); // Clears screen to black
    SDL_RenderClear(_displayRendererRefrence);

    SDL_SetRenderDrawColor(_displayRendererRefrence, 255, 255, 255, 255);

    for (int height = 0; height < DEFAULT_HEIGHT; height++)
    {
        for (int width = 0; width < DEFAULT_WIDTH; width++)
        {
            r.y = height * DISPLAY_SCALE_FACTOR;
            r.x = width * DISPLAY_SCALE_FACTOR;

            if (_displayArray[height][width])
            {
                SDL_RenderFillRect(_displayRendererRefrence, &r);
            }
        }
    }

    SDL_RenderPresent(_displayRendererRefrence);
}

/*
    Reads n bytes from memorry and draws a sprite to position (x,y)
*/
bool Display::drawSprite(uint16_t x, uint16_t y, uint16_t address, uint16_t numberOfBytes)
{
    uint8_t sprite[15] = { 0 };

    for (int i = 0; i < numberOfBytes; i++)
    {
        sprite[i] = _mmuRefrence->readMemorryAddress(address + i);
    }

    bool collision = false;
    for (int byteNum = 0; byteNum < numberOfBytes; byteNum++)
    {
        for (int bitNum = 0; bitNum < 8; bitNum++)
        {
            uint8_t currentPixle = (sprite[byteNum] << bitNum) & 0x1;
            bool currentPixleState = _displayArray[(y + byteNum) % DEFAULT_HEIGHT][(x + bitNum) % DEFAULT_WIDTH];
            if ((currentPixleState ^ currentPixleState) != currentPixleState)
            {
                collision = true;
            }
        }
    }

    draw();
    return collision;
}