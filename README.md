# Mazegen
Simple algorithm for maze generation with visualization.

## Reguirements
- SDL2

## Example maze
The following example is animated by simply inserting some delay into the maze generation algorithm and drawing the whole thing with every step.

<img src="https://raw.githubusercontent.com/c3r/mazegen/master/anim.gif" width="797">

## Archipelago generation
Apparently, when enough fiddled with, this kind of maze generation algorithm can be also used to generate a 2D graphics representation of islands archipelago. The intermediate product which is yielded after some fraction of work being done on the maze generation looks like a complicated island archipelago.

The following picutre was generated by simply stopping the mazegen algorithm after visiting 65% of all maze cells.
<img src="https://raw.githubusercontent.com/c3r/mazegen/master/archi.PNG">

## TODO
- user customization
- random blind spots for more realistic dungeon crawler level generation
- BST (stateless) version?
