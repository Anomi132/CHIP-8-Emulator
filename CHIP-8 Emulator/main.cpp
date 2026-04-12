#define SDL_MAIN_USE_CALLBACKS 1  
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>

#include "include/Emulator.hpp"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static double lastFrameTime = 0.0;
static double currentFrameTime = 0.0;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/clear", 640, 480, SDL_WINDOW_NOT_FOCUSABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, 640, 480, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    return SDL_APP_CONTINUE;  
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  
    }
    return SDL_APP_CONTINUE; 
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
    static Uint64 lastTime = 0;
    Uint64 currentTime = SDL_GetTicksNS();

    if (lastTime == 0) lastTime = currentTime;

    double deltaTime = (double)(currentTime - lastTime) / 1000000000.0;

    Emulator::update(deltaTime);
        
    lastTime = currentTime;

    return SDL_APP_CONTINUE;  
}


void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    
}
