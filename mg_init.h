#pragma once
#include <SDL.h>
#include <iostream>

extern SDL_Window* g_MazeWindow;
extern SDL_Renderer* g_MazeRenderer;

bool DrawInit(int w, int h);