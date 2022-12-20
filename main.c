#include "stdafx.h"
#include "snake.h"

char *grid = NULL;
const int DIM_X = 25, DIM_Y = 20;
int score = 0, run = 1;
char *buf = NULL;
HANDLE outHandle;

void sigint_handler(int sig) {
    run = 0;
}

void init() {
    printf("\x1b[2J");
    outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    grid = (char*) malloc(DIM_X * DIM_Y);
    memset(grid, GROUND, DIM_X * DIM_Y);
    buf = (char*) malloc(((DIM_X * 2) + 4) * (DIM_Y + 2));
    signal(SIGINT, sigint_handler);
    srand(time(NULL));
    initSnake();
}

void destroy() {
    if (buf) {
        free(buf);
    }
    if (grid) {
        free(grid);
    }
}

long long microseconds() {
    LARGE_INTEGER fq, t;
    QueryPerformanceFrequency(&fq);
    QueryPerformanceCounter(&t);
    return (1000000 * t.QuadPart) / fq.QuadPart;
}

int main() {
    init();
    int dir = -1;
    long long start = microseconds();
    while (run) {
        showGrid();
        while (run) {
            if (microseconds() - start > 100000) {
                start = microseconds();
                update(dir);
                showGrid();
            }
            if (kbhit()) {
                break;
            }
        }
        if (run) {
            switch (getch()) {
            case (72):
                dir = UP;
                break;
            case (77):
                dir = RIGHT;
                break;
            case (80):
                dir = DOWN;
                break;
            case (75):
                dir = LEFT;
                break;
            }
        }
    }
    destroy();
    destroySnake();
    printf("\nThanks for playing!\n");
    return 0;
}