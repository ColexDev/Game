#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>

#define NUM_OF_ENEMIES 3

void print_coords(int x, int y, int max_x)
{
    char buff[4];
    sprintf(buff, "%d", x);
    mvprintw(0, max_x - 7, buff);
    sprintf(buff, "%d", y);
    mvprintw(0, max_x - 3, buff);
}

bool collision_detection(int x, int y, int c_x, int c_y)
{
    return ((x == c_x && y == c_y) || (x == c_x - 2 && y == c_y) || (x == c_x - 1 && y == c_y));
}

bool create_enemy(int x, int y, int c_x, int c_y)
{
    mvprintw(c_y, c_x, "x");
    if (collision_detection(x, y, c_x, c_y)) {
        clear();
        mvprintw(1, 1, "You died, RIP");
        return true;
    } else {
        return false;
    }
}

bool create_enemies(int x, int y, const int num_of_enemies, const int locations[][2])
{
    bool result = false;

    for (int i = 0; i < num_of_enemies; i++) {
        result = create_enemy(x, y, locations[i][0], locations[i][1]);
        if (result) {
            break;
        }
    }
    return result;
}

void game_loop(int x, int y, int max_x, int max_y, char* character)
{
    while (1) {
        /* running getmaxyx in the while loop
         * allows for window resizing */
        getmaxyx(stdscr, max_y, max_x);

        clear();
        attron(A_BOLD);
        mvprintw(y, x, character);
        print_coords(x, y, max_x);
        attroff(A_BOLD);

        /* Creates all of the enemies and
         * checks for collision */
        int locations_of_enemies[NUM_OF_ENEMIES][2] = {
            {5, 5},
            {8, 9},
            {35, 26}
        };
        if (create_enemies(x, y, NUM_OF_ENEMIES, locations_of_enemies)) {
            getch();
            break;
        }

        /* Allows character movement with
         * WASD or HJKL */
        switch (getch()) {
        case 'j':
        case 's':
            character = "\\o/";
            y++;
            break;
        case 'k':
        case 'w':
            character = "/o\\";
            y--;
            break;
        case 'l':
        case 'd':
            character = "|o>";
            // x += 2;
            x++;
            break;
        case 'h':
        case 'a':
            character = "<o|";
            // x -= 2;
            x--;
            break;
        case 'q':
            endwin();
            exit(1);
            break;
        }

        /* Bounds checking */
        if (x >= max_x - 2) {
            x = max_x - 3;
        } else if (y >= max_y) {
            y--;
        } else if (x < 0) {
            x = 0;
        } else if (y < 0) {
            y = 0;
        }
    }
}

int main()
{
    /* Basic variable init */
    int x = 0, y = 0;
    int max_x = 0, max_y = 0;
    int next_x = 0;
    int direction = 1;
    char* character = "o";

    /* Inits ncurses */
    initscr();
    noecho();
    curs_set(FALSE);

    /* Runs the actual game*/
    game_loop(x, y, max_x, max_y, character);

    endwin();
}
