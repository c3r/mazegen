#pragma once
#include <SDL.h>

extern SDL_Renderer* MazeRenderer;
extern const int PIXEL_W;

void SetDrawColor(const SDL_Color& c) {
  SDL_SetRenderDrawColor(MazeRenderer, c.r, c.g, c.b, c.a);
}

void DrawRect(int x, int y, int w, int h, const SDL_Color& c) {
  SetDrawColor(c);
  SDL_Rect r = {x * PIXEL_W, y * PIXEL_W, w * PIXEL_W, h * PIXEL_W};
  SDL_RenderFillRect(MazeRenderer, &r);
}

void FillRect(const SDL_Rect& rect) { SDL_RenderFillRect(MazeRenderer, &rect); }
void ClearScreen() { SDL_RenderClear(MazeRenderer); }
void DrawScreen() { SDL_RenderPresent(MazeRenderer); }