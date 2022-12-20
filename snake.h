#pragma once

extern void showGrid();

extern void initSnake();

extern void update(int dir);

extern void destroySnake();

enum {UP=0,RIGHT, DOWN, LEFT};