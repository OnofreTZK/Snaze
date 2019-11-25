# A Snake Trapped in a Maze

## About the project
Run the classic snake game inside a maze! The challenge? Provide to snake the correct direction to the pellet. The path finder algorithm used here is Trémaux's without [graph theory](https://en.wikipedia.org/wiki/Tr%C3%A9maux_tree)( maybe one day ) 

## Game Rules
1. The snake has 5 lives.

2. The snake moves by extending its head 1 step in the direction it’s moving and pulling its tail in.

3. If the snake eats a piece of food, its length grows by one and a new food pellet is randomly placed in a location the snake can get to.

4. There might be one or more levels, depending on the input file.

5. Everytime the snake crashes into itself or into a wall it looses one life; The snake loses the game simulation when all 5 lives are spent.

6. The snake wins the game simulation if it eats all the food pellets in all levels.

## Compile and Run

Downloading:
```bash
$ git clone https://github.com/OnofreTZK/Snaze.git
``` 

In the root:
```bash
$ cmake -S . -Bbuild && build

$ make
```

