#include "stdafx.h"
#include "snake.h"

extern char *grid;
extern int score, run;
extern const int DIM_X, DIM_Y;
extern char *buf;
extern HANDLE outHandle;

typedef struct {
    int xOffset, yOffset;
} direction;

typedef struct {
    int x, y;
} position;

typedef struct {
    position pos;
    int dir;
} player;

static player pl;
static int *board;
static direction dirs[] = {{0,-1},{1,0},{0,1},{-1,0}};

static int isValid(int x, int y) {
    return !(x < 0 || y < 0 || x >= DIM_X || y >= DIM_Y);
}

static void setGridChar(int x, int y, char a) {
#ifdef DEBUG
    if (!isValid(x, y)) {
        LOG("ERROR: setGridChar function failed -> position out of bounds\n");
        return;
    }
#endif
    grid[(DIM_X * y) + x] = a;
}

static void setBoardVal(int x, int y, int a) {
#ifdef DEBUG
    if (!isValid(x, y)) {
        LOG("ERROR: setBoardVal function failed -> position out of bounds\n");
        return;
    }
#endif
    board[(DIM_X * y) + x] = a;
}

static char getGridChar(int x, int y) {
#ifdef DEBUG
    if (!isValid(x, y)) {
        LOG("ERROR: getGridChar function failed -> position out of bounds\n");
        return -1;
    }
#endif
    return grid[(DIM_X * y) + x];
}

static char getBoardVal(int x, int y) {
#ifdef DEBUG
    if (!isValid(x, y)) {
        LOG("ERROR: getBoardVal function failed -> position out of bounds\n");
        return -1;
    }
#endif
    return board[(DIM_X * y) + x];
}

void showGrid() {
    const int size = sizeof(char) * ((2 * DIM_X) + 4) * (DIM_Y + 2);
    memset(buf, '-', size);
    for (int i = 0; i < DIM_Y + 2; i++) {
        if (i == 0 || i == DIM_Y + 1) {
            buf[i * ((2 * DIM_X) + 4)] = buf[i * ((2 * DIM_X) + 4) + ((2 * DIM_X) + 2)] = '+';
        } else {
            for (int j = 0; j < DIM_X; j++) {
                buf[i * ((2 * DIM_X) + 4) + 2 + (j * 2)] = getGridChar(j, i - 1);
                buf[i * ((2 * DIM_X) + 4) + 1 + (j * 2)] = ' ';
            }
            buf[i * ((2 * DIM_X) + 4)] = buf[i * ((2 * DIM_X) + 4) + ((2 * DIM_X) + 2)] = '|';
            buf[i * ((2 * DIM_X) + 4) + ((2 * DIM_X) + 1)] = ' ';
        }
        buf[i * ((2 * DIM_X) + 4) + ((2 * DIM_X) + 3)] = '\n';
    }
    printf("\x1b[H\nScore: %d\n", score);
    WriteConsole(outHandle, buf, size, 0, 0);
    fflush(stdout);
}

void initSnake() {
    board = (int*) malloc(sizeof(int) * DIM_X * DIM_Y);
    memset(board, 0, sizeof(int) * DIM_X * DIM_Y);
    pl = (player) {(position) {(rand() % DIM_X), (rand() % DIM_Y)}, -1};
    position apple = (position) {(rand() % DIM_X), (rand() % DIM_Y)};
    while (apple.x == pl.pos.x && apple.y == pl.pos.y) {
        apple = (position) {(rand() % DIM_X), (rand() % DIM_Y)};
    }
    setGridChar(pl.pos.x, pl.pos.y, SNAKE);
    setBoardVal(pl.pos.x, pl.pos.y, INIT_LENGTH);
    setGridChar(apple.x, apple.y, APPLE);
}

void destroySnake() {
    if (board) {
        free(board);
    }
}

static void ateApple() {
    score += 100;
    for (int i = 0; i < DIM_Y; i++) {
        for (int j = 0; j < DIM_X; j++) {
            int temp = getBoardVal(j, i);
            if (temp > 0) {
                setBoardVal(j, i, temp + INIT_LENGTH);
            }
        }
    }
    int i = 10;
    position apple = (position) {(rand() % DIM_X), (rand() % DIM_Y)};
    while (i && getBoardVal(apple.x, apple.y) > 0) {
        apple = (position) {(rand() % DIM_X), (rand() % DIM_Y)};
        i--;
    }
    int found = 1;
    if (i == 0) {
        found = 0;
        for (; i < DIM_Y; i++) {
            for (int j = 0; j < DIM_X; j++) {
                if (getBoardVal(j, i) == 0) {
                    apple = (position) {j, i};
                    found = 1;
                    break;
                }
            }
            if (found) {
                break;
            }
        }
    }
    if (found) {
        setGridChar(apple.x, apple.y, APPLE);
    }
}

void update(int dir) {
    if (dir >= 0) {
        for (int i = 0; i < DIM_Y; i++) {
            for (int  j = 0; j < DIM_X; j++) {
                int temp = getBoardVal(j, i);
                if (temp > 1) {
                    setBoardVal(j, i, temp - 1);
                } else if (temp == 1) {
                    setBoardVal(j, i, temp - 1);
                    setGridChar(j, i, GROUND);
                }
            }
        }
        if (dir + 2 == pl.dir || (dir + 2 == 5 && pl.dir == 1) || (dir + 2 == 4 && pl.dir == 0)) {
            dir = pl.dir;
        } else {
            pl.dir = dir;
        }
        pl.pos.x += dirs[dir].xOffset;
        pl.pos.y += dirs[dir].yOffset;
        if (isValid(pl.pos.x, pl.pos.y) && getGridChar(pl.pos.x, pl.pos.y) != SNAKE) {
            setBoardVal(pl.pos.x, pl.pos.y, INIT_LENGTH + ((score / 100) * INIT_LENGTH));
            if (getGridChar(pl.pos.x, pl.pos.y) == APPLE) {
                ateApple();
            }
            setGridChar(pl.pos.x, pl.pos.y, SNAKE);
        } else {
            run = 0;
        }
    }
}