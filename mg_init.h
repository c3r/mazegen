#pragma once
#include <SDL.h>
#include <iostream>

extern SDL_Window* MazeWindow;
extern SDL_Renderer* MazeRenderer;

bool init(int w, int h);