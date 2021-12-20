#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <pthread.h>

#define NUM_OF_ENEMIES 3
#define LEFT_CHARACTER "<o|"
#define RIGHT_CHARACTER "|o>"
#define DOWN_CHARACTER "\\o/"
#define UP_CHARACTER "/o\\"

/* Basic variable init */
int x = 0, y = 0;
int max_x = 0, max_y = 0;
int next_x = 0;
int direction = 1;
char* character = "o";

int locations_of_enemies[NUM_OF_ENEMIES][2] = {
    {5, 5},
    {8, 9},
    {35, 26}
};


void print_coords(int x, int y, int max_x)
{
    char buff[4];
    sprintf(buff, "%d", x);
    mvprintw(0, max_x - 7, buff);
    sprintf(buff, "%d", y);
    mvprintw(0, max_x - 3, buff);
}

void * create_bullet()
{
    int b_x;
    while (b_x < max_x - 1) {
        clear();
        mvprintw(y, b_x, "-");
        usleep(5000);
        b_x++;
        refresh();
    }
}

bool collision_detection(int x, int y, int c_x, int c_y)
{
    return ((x == c_x && y == c_y) || (x == c_x - 2 && y == c_y) || (x == c_x - 1 && y == c_y));
}

bool create_enemy(int c_x, int c_y)
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

bool create_enemies()
{
    bool result = false;

    for (int i = 0; i < NUM_OF_ENEMIES; i++) {
        result = create_enemy(locations_of_enemies[i][0], locations_of_enemies[i][1]);
        if (result) {
            break;
        }
    }
    return result;
}

void game_loop()
{
    pthread_t bullet_thread;
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
        if (create_enemies()) {
            getch();
        }

        /* Allows character movement with
         * WASD or HJKL */
        switch (getch()) {
        case 'j':
        case 's':
            character = DOWN_CHARACTER;
            y++;
            break;
        case 'k':
        case 'w':
            character = UP_CHARACTER;
            y--;
            break;
        case 'l':
        case 'd':
            character = RIGHT_CHARACTER;
            // x += 2;
            x++;
            break;
        case 'h':
        case 'a':
            character = LEFT_CHARACTER;
            // x -= 2;
            x--;
            break;
        case 'b':
            pthread_create(&bullet_thread, NULL, create_bullet, NULL);
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
    /* Inits ncurses */
    initscr();
    noecho();
    curs_set(FALSE);

    game_loop();

    endwin();
}
