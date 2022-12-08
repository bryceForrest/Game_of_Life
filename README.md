# John Conway's Game of Life
![Game of Life](/life/life.gif)

A C++ program that generates a random instanc of John Conway's Game of Life to be played out on the terminal.

The rules of the game of life are as follows:
* Any live cell with fewer than two live neighbours dies, as if by underpopulation.
* Any live cell with two or three live neighbours lives on to the next generation.
* Any live cell with more than three live neighbours dies, as if by overpopulation.
* Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.

(per Wikipedia, https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)

This implementation uses `ncurses` library and takes slight advantage of multithreading, by calculating the next iteration while the current iteration is being printed.