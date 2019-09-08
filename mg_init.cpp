#include "mg_init.h"

bool init(int w, int h)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // Set texture filtering to linear
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        printf("Warning: Linear texture filtering not enabled!");
    }

    MazeWindow = SDL_CreateWindow("Mazegen v0.0.1", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);

    if (MazeWindow == NULL) {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // Create renderer for window
    MazeRenderer = SDL_CreateRenderer(MazeWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(MazeRenderer, SDL_BLENDMODE_BLEND);

    if (MazeRenderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}