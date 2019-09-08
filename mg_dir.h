#pragma once
#include <cstdint>

extern const uint16_t k_MazeHeight;
extern const uint16_t k_MazeWidth;

enum Direction
{
  N_WALL = 0x01,
  E_WALL = 0x02,
  S_WALL = 0x04,
  W_WALL = 0x08
};

constexpr int VISITED = 0x10;

struct Point {
  uint32_t x, y;
};

inline bool NorthBound(Point& p) { return p.y > 0; }
inline bool WestBound(Point& p) { return p.x > 0; }
inline bool SouthBound(Point& p) { return p.y < k_MazeHeight - 1; }
inline bool EastBound(Point& p) { return p.x < k_MazeWidth - 1; }

inline int At(const Point &p) { return p.y * k_MazeWidth + p.x; }
inline Point NorthOf(Point& p) { return { p.x, (p.y - 1) }; }
inline Point EastOf(Point& p) { return { (p.x + 1), p.y }; }
inline Point SouthOf(Point& p) { return { p.x, (p.y + 1) }; }
inline Point WestOf(Point& p) { return { (p.x - 1), p.y }; }

constexpr int ALL_WALL = N_WALL | E_WALL | S_WALL | W_WALL;
Point (*dir_func[ALL_WALL])(Point& p) = { nullptr, NorthOf,    EastOf,    nullptr, SouthOf,    nullptr, nullptr, nullptr, WestOf };
bool  (*bfunc[ALL_WALL])(Point& p)    = { nullptr, NorthBound, EastBound, nullptr, SouthBound, nullptr, nullptr, nullptr, WestBound };