#pragma once
#include <SDL.h>
#include <stdio.h>
#include <time.h>
#include <random>
#include <stack>
#include <vector>

const uint8_t k_MazeWidth = 40;
const uint8_t k_MazeHeight = 20;

const uint8_t k_CellWidth = 4;
const uint8_t k_CellHeight = 4;
const uint8_t k_WallSize = 1;
const uint8_t k_CellAndWallWidth = k_CellWidth + k_WallSize;

const int k_DrawDelay = 25;