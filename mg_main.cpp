#include <SDL.h>
#include <stdio.h>
#include <time.h>
#include <random>
#include <stack>
#include <vector>
#include "mg_draw.h"
#include "mg_init.h"

#define neighbour dfunc[dir] 

const int k_PixelWidth = 5;
constexpr uint8_t k_MazeWidth = 12;
constexpr uint8_t k_MazeHeight = 12;

inline bool NorthBound	(uint32_t x, uint32_t y) { return y > 0; }
inline bool WestBound		(uint32_t x, uint32_t y) { return x > 0; }
inline bool SouthBound	(uint32_t x, uint32_t y) { return y < k_MazeHeight - 1; }
inline bool EastBound		(uint32_t x, uint32_t y) { return x < k_MazeWidth - 1; }

inline int At				(uint32_t x, uint32_t y)	{ return y * k_MazeWidth + x; }
inline int NorthOf	(uint32_t x, uint32_t y)	{ return At(x, (y - 1)); }
inline int EastOf		(uint32_t x, uint32_t y)	{ return At((x + 1), y); }
inline int SouthOf	(uint32_t x, uint32_t y)	{ return At(x, (y + 1)); }
inline int WestOf		(uint32_t x, uint32_t y)	{ return At((x - 1), y); }

enum Direction { N_WALL = 0x01, E_WALL = 0x02, S_WALL = 0x04, W_WALL = 0x08 };

constexpr int k_CellWidth = 4;
constexpr int k_CellHeight = 4;
constexpr int k_WallWidth = 1;
constexpr int k_CellAndWallWidth = k_CellWidth + k_WallWidth;

constexpr int k_ScreenWidth = k_MazeWidth * k_CellAndWallWidth * k_PixelWidth -
(k_WallWidth * k_PixelWidth);

constexpr int k_ScreenHeight =
k_MazeHeight * k_CellAndWallWidth * k_PixelWidth -
(k_WallWidth * k_PixelWidth);

constexpr int k_ScreenX = 10;
constexpr int k_ScreenY = 25;

const SDL_Color k_NotVisitedColor = { 0xFF, 0x00, 0x00, 0xFF };
const SDL_Color k_WallColor = { 0x00, 0xFF, 0x00, 0xFF };
const SDL_Color k_VisitedColor = { 0xFF, 0xFF, 0x00, 0xFF };

const int k_DrawDelay = 50;

SDL_Window* g_MazeWindow = NULL;
SDL_Renderer* g_MazeRenderer = NULL;

struct cell
{
	int x, y;
	int8_t walls;
	bool visited;
};

int(*dfunc[N_WALL | E_WALL | S_WALL | W_WALL])(uint32_t, uint32_t) = { 
	nullptr,
	NorthOf, 
	EastOf, nullptr,
	SouthOf, nullptr, nullptr, nullptr,
	WestOf 
};

bool(*bfunc[N_WALL | E_WALL | S_WALL | W_WALL])(uint32_t, uint32_t) = {
	nullptr,
	NorthBound,
	EastBound, nullptr,
	SouthBound, nullptr, nullptr, nullptr,
	WestBound
};

void DrawCell(cell* c)
{
	int x, y;
	x = c->x * k_CellAndWallWidth;
	y = c->y * k_CellAndWallWidth;
	DrawRect(x, y, k_CellWidth, k_CellHeight, c->visited ? k_VisitedColor : k_NotVisitedColor);

	if (c->walls & E_WALL) DrawRect(x + k_CellWidth, y, k_WallWidth, k_CellHeight + k_WallWidth, k_WallColor); 
	else									 DrawRect(x + k_CellWidth, y, k_WallWidth, k_CellHeight, c->visited ? k_VisitedColor : k_NotVisitedColor); 

	if (c->walls & S_WALL) DrawRect(x, y + k_CellHeight, k_CellWidth + k_WallWidth, k_WallWidth, k_WallColor); 
	else									 DrawRect(x, y + k_CellHeight, k_CellWidth, k_WallWidth, c->visited ? k_VisitedColor : k_NotVisitedColor); 
}

void visit(cell* from, cell* to, Direction dir)
{
	switch (dir) {
		case N_WALL: to->walls	 &= ~S_WALL; break;
		case E_WALL: from->walls &= ~E_WALL; break;
		case S_WALL: from->walls &= ~S_WALL; break;
		case W_WALL: to->walls	 &= ~E_WALL; break;
	}
	to->visited = true;
}

void DrawMaze(cell* maze) 
{
	for (uint8_t x = 0; x < k_MazeWidth; x++)
		for (uint8_t y = 0; y < k_MazeHeight; y++) 
			DrawCell(&maze[At(x, y)]);
	
	DrawScreen();
}

void InitFirstCell(std::stack<cell*> &stack, cell* maze)
{
	int x, y, place;
	x = rand() % k_MazeWidth;
	y = rand() % k_MazeHeight;
	place = At(x, y);
	stack.push(&maze[place]);
	maze[place].visited = true;
}

void InitMaze(cell* maze)
{
	for (uint8_t y = 0; y < k_MazeWidth; y++)
		for (uint8_t x = 0; x < k_MazeHeight; x++) 
			maze[At(x, y)] = { x, y, (N_WALL | E_WALL | S_WALL | W_WALL), false };
}

void InitNeighbour(uint32_t x, uint32_t y, cell* maze, std::vector<Direction> &neighbours, Direction dir) {
	bool in_bound, visited;
	
	in_bound = bfunc[dir](x, y);
	visited = maze[dfunc[dir](x, y)].visited;
	
	if (in_bound && !visited)
		neighbours.push_back(dir);
}

void InitNeighbours(uint32_t x, uint32_t y, cell* maze, std::vector<Direction>& neighbours)
{
	InitNeighbour(x, y, maze, neighbours, N_WALL);
	InitNeighbour(x, y, maze, neighbours, E_WALL);
	InitNeighbour(x, y, maze, neighbours, S_WALL);
	InitNeighbour(x, y, maze, neighbours, W_WALL);
}

int main(int argc, char** argv)
{
	srand(time(NULL));
	std::stack<cell*> stack;
	cell* maze = new cell[k_MazeWidth * k_MazeHeight];
	int visited;

	if (!DrawInit(k_ScreenWidth, k_ScreenHeight))
	{
		printf("Initialization failed! Exiting...");
		return 1;
	}

	InitMaze(maze);
	InitFirstCell(stack, maze);
	visited = 1;

	while (visited < k_MazeWidth * k_MazeHeight)
	{
		std::vector<Direction> neighbours;
		int y, x, curr_index, neighbour_index;
		Direction dir;
		cell* dest;
		cell* curr;

		y = stack.top()->y;
		x = stack.top()->x;
		curr_index = At(x, y);

		InitNeighbours(x, y, maze, neighbours);
					
		if (!neighbours.empty())
		{
			dir = neighbours.at(rand() % neighbours.size());
			neighbour_index = dfunc[dir](x, y);
			
			dest = &maze[neighbour_index];
			curr = &maze[curr_index];
			
			visit(curr, dest, dir);
			stack.push(dest);
			
			visited++;
			SDL_Delay(k_DrawDelay);
		} 
		else
			stack.pop();
		
		DrawMaze(maze);
	}

	SDL_Delay(5000);
	SDL_DestroyWindow(g_MazeWindow);
	SDL_Quit();
	return 0;
}