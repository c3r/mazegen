#pragma once
#include <SDL.h>

extern SDL_Renderer* g_MazeRenderer;
extern const int k_PixelWidth;

void SetDrawColor(const SDL_Color& c) {
  SDL_SetRenderDrawColor(g_MazeRenderer, c.r, c.g, c.b, c.a);
}

void DrawRect(int x, int y, int w, int h, const SDL_Color& c) {
  SetDrawColor(c);
  SDL_Rect r = { x * k_PixelWidth, y * k_PixelWidth, w * k_PixelWidth, h * k_PixelWidth };
  SDL_RenderFillRect(g_MazeRenderer, &r);
}

void FillRect(const SDL_Rect& rect) { SDL_RenderFillRect(g_MazeRenderer, &rect); }
void ClearScreen() { SDL_RenderClear(g_MazeRenderer); }
void DrawScreen() { SDL_RenderPresent(g_MazeRenderer); }