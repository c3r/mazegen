#include "mg_draw.h"
#include "mg_init.h"
#include <SDL.h>
#include <random>
#include <stack>
#include <stdio.h>
#include <time.h>
#include <vector>

constexpr uint8_t MAZE_WIDTH = 32;
constexpr uint8_t MAZE_HEIGHT = 32;

const int PIXEL_W = 5;
constexpr int CELL_W = 4;
constexpr int CELL_H = 4;
constexpr int WALL_W = 1;
constexpr int CELL_AND_WALL = CELL_W + WALL_W;

constexpr int SCREEN_W = MAZE_WIDTH * CELL_AND_WALL * PIXEL_W - (WALL_W * PIXEL_W);
constexpr int SCREEN_H = MAZE_HEIGHT * CELL_AND_WALL * PIXEL_W - (WALL_W * PIXEL_W);
constexpr int SCREEN_X = 10;
constexpr int SCREEN_Y = 25;

const SDL_Color CELL_COLOR = { 0xFF, 0x00, 0x00, 0xFF };
const SDL_Color WALL_COLOR = { 0x00, 0xFF, 0x00, 0xFF };
const SDL_Color VISITED_COLOR = { 0xFF, 0xFF, 0x00, 0xFF };

const int DELAY = 50;

SDL_Window* MazeWindow = NULL;
SDL_Renderer* MazeRenderer = NULL;

enum {
    N_WALL = 0x01,
    W_WALL = 0x02,
    S_WALL = 0x04,
    E_WALL = 0x08
};

enum {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3,
};

struct cell {
    int x;
    int y;
    bool n = true;
    bool w = true;
    bool s = true;
    bool e = true;
    bool visited;
};

void DrawCell(cell* cell)
{
    int x = cell->x * CELL_AND_WALL;
    int y = cell->y * CELL_AND_WALL;
    DrawRect(x, y, CELL_W, CELL_H, cell->visited ? VISITED_COLOR : CELL_COLOR);

    int w, h;
    SDL_Color c;

    if (cell->e) {
        h = CELL_H + WALL_W;
        c = WALL_COLOR;
    } else {
        h = CELL_H;
        c = cell->visited ? VISITED_COLOR : CELL_COLOR;
    }
    DrawRect(x + CELL_W, y, WALL_W, h, c);

    if (cell->s) {
        w = CELL_W + WALL_W;
        c = WALL_COLOR;
    } else {
        w = CELL_W;
        c = cell->visited ? VISITED_COLOR : CELL_COLOR;
    }

    DrawRect(x, y + CELL_H, w, WALL_W, c);
}

int at(int x, int y)
{
    return y * MAZE_WIDTH + x;
}

int main(int argc, char** argv)
{
    srand(time(NULL));
    cell* maze = new cell[MAZE_WIDTH * MAZE_HEIGHT];
    //memset(maze, 0x00, MAZE_WIDTH * MAZE_HEIGHT * sizeof(cell));
    for (uint8_t i = 0; i < MAZE_WIDTH; i++) {
        for (uint8_t j = 0; j < MAZE_HEIGHT; j++) {
            cell c;
            c.x = i;
            c.y = j;
            c.visited = false;
            maze[at(i, j)] = c;
        }
    }

    if (!init(SCREEN_W, SCREEN_H)) {
        printf("Initialization failed! Exiting...");
        return 1;
    }

    std::stack<cell*> stack;

    // TODO: randomize
    cell fc;
    fc.x = rand() % MAZE_WIDTH;
    fc.y = rand() % MAZE_HEIGHT;
    stack.push(&fc);
    maze[at(fc.x, fc.y)].visited = true;

    int visitedCells = 1;
    while (visitedCells < MAZE_WIDTH * MAZE_HEIGHT) {

        // Populate neighbours ===============================================
        std::vector<uint8_t> neighbours;
        int y = stack.top()->y;
        int x = stack.top()->x;

#define GT_NORTH (y > 0)
#define GT_WEST (x > 0)
#define LT_SOUTH (y < MAZE_HEIGHT - 1)
#define LT_EAST (x < MAZE_WIDTH - 1)

#define AT(x, y) y* MAZE_WIDTH + x
#define AT_NORTH AT(x, (y - 1))
#define AT_EAST AT((x + 1), y)
#define AT_SOUTH AT(x, (y + 1))
#define AT_WEST AT((x - 1), y)
#define AT_PLACE AT((x), (y))

        if (GT_NORTH && !maze[AT_NORTH].visited) 
            neighbours.push_back(NORTH);
        
        if (LT_EAST && !maze[AT_EAST].visited) 
            neighbours.push_back(EAST);
        
        if (LT_SOUTH && !maze[AT_SOUTH].visited) 
            neighbours.push_back(SOUTH);
        
        if (GT_WEST && !maze[AT_WEST].visited) 
            neighbours.push_back(WEST);
        
        cell* me = &maze[AT_PLACE];
        if (!neighbours.empty()) {
            // Randomly select one neighbour =================================
            uint8_t direction = neighbours.at(rand() % neighbours.size());
            switch (direction) {
            case NORTH: {
                cell* north = &maze[at(x, y - 1)];
                north->s = false;
                north->visited = true;
                me->n = false;
                stack.push(north);
                break;
            }
            case EAST: {
                cell* east = &maze[at(x + 1, y)];
                east->w = false;
                east->visited = true;
                me->e = false;
                stack.push(east);
                break;
            }
            case SOUTH: {
                cell* south = &maze[at(x, y + 1)];
                south->n = false;
                south->visited = true;
                me->s = false;
                stack.push(south);
                break;
            }
            case WEST: {
                cell* west = &maze[at(x - 1, y)];
                west->e = false;
                west->visited = true;
                me->w = false;
                stack.push(west);
                break;
            }
            }
            visitedCells++;
            SDL_Delay(DELAY);
        } else {
            if (stack.size() != 0)
                stack.pop();
        }

        // draw labirynth ====================================================
        for (uint8_t i = 0; i < MAZE_WIDTH; i++) {
            for (uint8_t j = 0; j < MAZE_HEIGHT; j++) {
                cell* c = &maze[at(i, j)];
                c->x = i;
                c->y = j;
                DrawCell(c);
            }
        }
        DrawScreen();
    }

    SDL_Delay(5000);
    // Quit ====================================================
    SDL_DestroyWindow(MazeWindow);
    SDL_Quit();
    return 0;
}