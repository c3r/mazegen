#pragma once
#include <SDL.h>
#include "mg_dir.h"
#include <iostream>

extern SDL_Renderer* g_MazeRenderer;

struct MazeCell;

constexpr int k_PixelSize = 1;
constexpr int k_ScreenX = 5;
constexpr int k_ScreenY = 25;

constexpr SDL_Color k_NotVisitedColor = { 0x00, 0xFF, 0x00, 0xAA };
constexpr SDL_Color k_VisitedColor = { 0x00, 0x00, 0xFF, 0xFF };
constexpr SDL_Color k_WallColor = k_VisitedColor;//{ 0xFF, 0xCC, 0x00, 0xFF };
constexpr SDL_Color k_sWallColor = k_WallColor;

SDL_Window* g_MazeWindow;
SDL_Renderer* g_MazeRenderer;

bool DrawInit(int maze_width, int maze_height, int cell_width, int cell_height, int wall_size)
{
  int16_t sw = maze_width * (wall_size + cell_width) * k_PixelSize - (wall_size * k_PixelSize);
  int16_t sh = maze_height * (wall_size + cell_height) * k_PixelSize - (wall_size * k_PixelSize);

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    return false;

  if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2"))
    printf("Warning: Linear texture filtering not enabled!");

  std::cout << "Initializing window size " << sw << "x" << sh << "px...";
  g_MazeWindow = SDL_CreateWindow("Mazegen v0.0.1", SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED, sw, sh, SDL_WINDOW_SHOWN);

  if (g_MazeWindow == NULL)
    return false;
  std::cout << "Done. " << std::endl;

  g_MazeRenderer = SDL_CreateRenderer(g_MazeWindow, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawBlendMode(g_MazeRenderer, SDL_BLENDMODE_BLEND);

  if (g_MazeRenderer == NULL)
    return false;

  return true;
}

void SetDrawColor(const SDL_Color& c) { SDL_SetRenderDrawColor(g_MazeRenderer, c.r, c.g, c.b, c.a); }
void DrawRect(int x, int y, int w, int h, const SDL_Color& c)
{
  SetDrawColor(c);
  SDL_Rect r = { x * k_PixelSize, y * k_PixelSize, w * k_PixelSize, h * k_PixelSize };
  SDL_RenderFillRect(g_MazeRenderer, &r);
}
void FillRect(const SDL_Rect& rect) { SDL_RenderFillRect(g_MazeRenderer, &rect); }
void ClearScreen() { SDL_RenderClear(g_MazeRenderer); }
void DrawScreen() { SDL_RenderPresent(g_MazeRenderer); }

void DrawCell(int x, int y, int walls, int cell_width, int cell_height, int wall_size, bool visited)
{
  x *= cell_width + wall_size;
  y *= cell_height + wall_size;

  SDL_Color col = k_NotVisitedColor;
  col.a = rand() % 0x44 + 0xbb ;

  DrawRect(x, y, cell_width, cell_height, visited ? k_VisitedColor : col);

  if (walls & E_WALL)
    DrawRect(x + cell_width, y, wall_size, cell_height + wall_size, k_WallColor);
  else
    DrawRect(x + cell_width, y, wall_size, cell_height, visited ? k_VisitedColor : k_NotVisitedColor);

  if (walls & S_WALL)
    DrawRect(x, y + cell_height, cell_width + wall_size, wall_size, k_sWallColor);
  else
    DrawRect(x, y + cell_height, cell_width, wall_size, visited ? k_VisitedColor : k_NotVisitedColor);

  DrawRect(x + cell_width, y + cell_height, wall_size, wall_size, k_WallColor);
}

void DrawCell(int x, int y, int cell_width, int walls, int wall_size, int cell_height) { DrawCell(x, y, walls, cell_width, cell_height, wall_size, false); }
void DrawCellVisited(int x, int y, int cell_width, int walls, int wall_size, int cell_height) { DrawCell(x, y, walls, cell_width, cell_height, wall_size, true); }