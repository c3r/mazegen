# Mazegen
Simple algorithm for maze generation with visualization. Method used to generate the maze is called Recursive Backtracking. For the sake of memory efficency, no recursion was used. Only backtracking with a stack data structure.

## Reguirements
- SDL2

## Example maze
The following example is animated by simply inserting some delay into the maze generation algorithm and drawing the whole thing with every step.

<img src="https://raw.githubusercontent.com/c3r/mazegen/master/anim.gif" width="797">

## Archipelago generation
Apparently, when enough fiddled with, Recursive Backtracking algorithm can also be used to generate a 2D graphics representation of islands archipelago. The intermediate product which is yielded after some fraction of work being done on the maze generation looks like a complicated island archipelago.

The following picutre was generated by simply stopping the RB algorithm after visiting 65% of all maze cells.
<img src="https://raw.githubusercontent.com/c3r/mazegen/master/archi.PNG">

## TODO
- user customization (pixel size, maze size, colors, % of completion)
- BST (stateless) version
- Aldous Broder method
- Wilson method
- Hunt and Kill method
