#include "mg_draw.h"
#include "mg_main.h"
#include "mg_dir.h"

void InitFirstCell(std::stack<Point>& stack, int* maze)
{
  Point point = { rand() % k_MazeWidth, rand() % k_MazeHeight };
  stack.push(point);
  maze[At(point)] |= VISITED;
}

void InitMaze(int* maze)
{
  for (uint8_t x = 0; x < k_MazeWidth; x++)
    for (uint8_t y = 0; y < k_MazeHeight; y++)
      maze[At({ x, y })] = ( N_WALL | E_WALL | S_WALL | W_WALL );
}

void InitNeighbour(Point& src, int* maze, std::vector<Direction>& neighbours, Direction dir) {
  if (bfunc[dir](src))
  {
    Point npoint = dir_func[dir](src);
    int neighbour = maze[At(npoint)];
    if (!(neighbour & VISITED))
      neighbours.push_back(dir);
  }
}

void InitNeighbours(Point& src, int* maze, std::vector<Direction>& neighbours)
{
  InitNeighbour(src, maze, neighbours, N_WALL);
  InitNeighbour(src, maze, neighbours, E_WALL);
  InitNeighbour(src, maze, neighbours, S_WALL);
  InitNeighbour(src, maze, neighbours, W_WALL);
}

void DrawMaze(int* maze)
{
  int cell;
  for (uint8_t x = 0; x < k_MazeWidth; x++)
    for (uint8_t y = 0; y < k_MazeHeight; y++)
    {
      cell = maze[At({ x, y })];
      cell & VISITED
        ? DrawCellVisited(x, y, k_CellWidth, cell, k_WallSize, k_CellHeight)
        : DrawCell       (x, y, k_CellWidth, cell, k_WallSize, k_CellHeight);               
    }

  DrawScreen();
}

int main(int argc, char** argv)
{
  srand(time(NULL));
  std::stack<Point> stack;
  int* maze = new int[k_MazeWidth * k_MazeHeight];
  int visited;

  if (!DrawInit(k_MazeWidth, k_MazeHeight, k_CellWidth, k_CellHeight, k_WallSize))
  {
    printf("Initialization failed! Exiting...");
    return 1;
  }

  SDL_Delay(5000);

  InitMaze(maze);
  InitFirstCell(stack, maze);
  visited = 1;

  while (visited < k_MazeWidth * k_MazeHeight)
  {
    std::vector<Direction> neighbours;
    int *dest, *curr;
    Direction dir;
    Point dest_point;
    Point curr_point;

    curr_point = stack.top();
    InitNeighbours(curr_point, maze, neighbours);

    if (!neighbours.empty())
    {
      dir = neighbours.at(rand() % neighbours.size());
      Point dest_point = dir_func[dir](curr_point);
      
      dest = &maze[At(dest_point)];
      curr = &maze[At(curr_point)];

      if (dir == N_WALL) { *dest &= ~S_WALL; }
      if (dir == E_WALL) { *curr &= ~E_WALL; }
      if (dir == S_WALL) { *curr &= ~S_WALL; }
      if (dir == W_WALL) { *dest &= ~E_WALL; }
      *dest |= VISITED;

      stack.push(dest_point);

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