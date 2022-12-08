/// @file life.cpp
/// @author Bryce Forrest
/// @date 12/07/2022
/// @brief This program randomly generates John Conway's Game of Life
/// in the terminal using ncurses library.

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <ncurses.h>

using namespace std;
using Vector = std::vector<bool>;       // to store vectors of bools
using Matrix = std::vector<Vector>;     // to store vectors of vectors

void initialize();
bool checkArgs(int argCount, char* argVector[]);
void print(const Matrix board);
void life(Matrix& board);
size_t countNbors(const Matrix& board, size_t x, size_t y);
void randBoard(Matrix& board);

int main(int argc, char* argv[]) {
    int exitStatus = EXIT_SUCCESS;              // exit status for main

    if (checkArgs(argc, argv)) {
        size_t dim = stoi(argv[1]);             // board dimension
        Matrix board(dim, Vector(dim, false));  // initialize board to false

        // prepare ncurses, seed random, fill the board with random values
        initialize();
        randBoard(board);

        // loop interrupted by keypress 'q'
        // reset cursor position and print board. Multi-threading allows
        // function life() to prepare the next iteration while the board
        // is printing. The threads join before restarting loop.
        while (getch() != 'q') {
            move(0, 0);
            life(board);
            thread t1(print, board);
            thread t2(life, ref(board));
            t1.join();
            t2.join();
        }

        endwin();
    } else {
        exitStatus = EXIT_FAILURE;
    }

    return exitStatus;
}

/// @brief Sets up ncurses and seeds RNG
void initialize() {
    initscr();
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_GREEN);
    noecho();
    curs_set(FALSE);
    timeout(0);
    srand(time(0));
}

/// @brief Checks argv arguments and prints out relevant error message
///
/// @param      argCount    Argument count
/// @param      argVector   Argument vector
/// @return     True if no error was found, else false
bool checkArgs(int argCount, char* argVector[]) {
    bool valid = true;      // return value for function
    if (argCount != 2) {
        cout << "Usage: " << argVector[0] << " <board dimension>" << endl;
        valid = false;
    } else {
        int dim = stoi(argVector[1]);
        if (dim < 0 || dim > 40) {
            cout << "Board dimensions should be (0, 40]" << endl;
            valid = false;
        }
    }

    return valid;
}

/// @brief Prints board using ncurses
///
/// @param  board   board to be printed
/// @note   function expects to be called by a thread, sleeps for 500ms
///         for user display
void print(const Matrix board) {
    for (size_t y = 0; y < board.size(); ++y) {
        for (size_t x = 0; x < board.size(); ++x) {
            if (board[y][x] == true) {
                attron(COLOR_PAIR(1));
                addstr("  ");
                attroff(COLOR_PAIR(1));
            } else {
                addstr("  ");
            }
            refresh();
        }

        addstr("\n");
        refresh();
    }

    addstr("Press 'q' to quit.");
    refresh();

    this_thread::sleep_for(std::chrono::milliseconds(500ms));
}

/// @brief Performs the "Game of Life" rules on each square
///
/// @param  board   Board to be checked
void life(Matrix& board) {
    // copy vec into newVec, and make changes to newVec
    Matrix newBoard(board);

    for (size_t y = 0; y < board.size(); ++y) {
        for (size_t x = 0; x < board.size(); ++x) {
            // for each cell, count the live neighbors
            auto count = countNbors(board, x, y);

            // apply rules of "life"
            // If the cell is live, with less than two neighbors, the cell dies
            // of loneliness.
            // If there are greater than three neighbors, the cell dies of
            // overcrowdedness
            // If a non-live cell has exactly three neighbors, the cell comes to
            // life by reproduction I guess?

            if (board[y][x]) {
                if (count < 2 || count > 3) {
                    newBoard[y][x] = false;
                }
            } else {
                if (count == 3) {
                    newBoard[y][x] = true;
                }
            }
        }
    }

    // apply updated vector to vec
    board = newBoard;
}

/// @brief Counts "neighbors" in squares around current
///
/// @param  board   board to be checked
/// @param  x       x coordinate
/// @param  y       y coordinate
/// @return         Returns the number of neighbors
size_t countNbors(const Matrix& board, size_t x, size_t y) {
    size_t count {};
    int dim = static_cast<int>(board.size());

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int x_p = (static_cast<int>(x) + j) % dim;
            int y_p = (static_cast<int>(y) + i) % dim;

            x_p = x_p < 0 ? x_p + board.size() : x_p;
            y_p = y_p < 0 ? y_p + board.size() : y_p;
            if (board[y_p][x_p]) {
                ++count;
            }
        }
    }

    return count;
}

/// @brief Randomize the values of Game of Life board (sets to t/f)
///
/// @param  board   board to be randomized
void randBoard(Matrix& board) {
    for (size_t y = 0; y < board.size(); ++y) {
        for (size_t x = 0; x < board.size(); ++x) {
            board[y][x] = static_cast<bool>(rand() % 2);
        }
    }
}
