#include <assert.h>
#include <stdio.h>

#define WIDTH  20
#define HEIGHT 10

#ifdef _WIN32
    #include <windows.h>
    #define SLEEP_MS(ms) Sleep(ms)
#else
    #include <unistd.h>
    #define SLEEP_MS(ms) usleep((ms) * 1000)
#endif

#define CLEAR_SCREEN() printf("\x1b[2J\x1b[H")
#define HIDE_CURSOR() printf("\x1b[?25l")

typedef struct {
    char values[HEIGHT][WIDTH];
} Board;

Board parseBoard(char s[HEIGHT+1][WIDTH+1], char alive, char dead) {
    assert(alive != dead);
    Board board = {0};
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (s[y][x] == alive)
                board.values[y][x] = 1;
            else if (s[y][x] != dead)
                goto error;
        }
    }
    return board;

    error:
    return (Board){0};
}

void printBoard(Board board) {
    printf("\x1b[H");
    for (int y = 0; y < HEIGHT; y++) {
        char line[WIDTH] = {0};
        for (int x = 0; x < WIDTH; x++) {
            if (board.values[y][x]) line[x] = '#';
            else                    line[x] = '.';
        }
        printf("%.*s\n", WIDTH, line);
    }
}

void nextState(Board *board) {
    Board old = *board;
    
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int neighbours = 0;
            for (int dy = -1; dy <= 1; dy++) {
                int ny = (y + dy + HEIGHT) % HEIGHT;
                for (int dx = -1; dx <= 1; dx++) {
                    int nx = (x + dx + WIDTH) % WIDTH;
                    if (old.values[ny][nx]) neighbours++;
                }
            }

            if (old.values[y][x]) {
                neighbours--;
                if (neighbours < 2 || neighbours > 3) board->values[y][x] = 0;
                else                                  board->values[y][x] = 1;
            } else {
                if (neighbours == 3) board->values[y][x] = 1;
                else                 board->values[y][x] = 0;
            }
        }
    }
}

int main() {
    char template[HEIGHT+1][WIDTH+1] = {
        "....................",
        "..#.................",
        "...#................",
        ".###................",
        "....................",
        "....................",
        "....................",
        "....................",
        "....................",
        "...................."
    };

    Board board = parseBoard(template, '#', '.');
    CLEAR_SCREEN();
    HIDE_CURSOR();
    printBoard(board);

    while (1) {
        SLEEP_MS(50);
        nextState(&board);
        printBoard(board);
    }
}