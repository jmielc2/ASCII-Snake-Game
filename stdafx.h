#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <signal.h>
#include <windows.h>
#include <time.h>

#ifdef DEBUG
    #define LOG(...) fprintf(stderr, __VA_ARGS__)
#else
    #define LOG(...)
#endif

#define SNAKE '#'
#define APPLE 'O'
#define GROUND ' '
#define INIT_LENGTH 4