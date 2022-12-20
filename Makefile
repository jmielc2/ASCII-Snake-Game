DEBUG_FLAGS=-DDEBUG -Wall -g -Werror
RELEASE_FLAGS=-Wall -O3

all: release

debug: main.c snake.c
	gcc ${DEBUG_FLAGS} main.c snake.c -o snake-d

release: main.c snake.c
	gcc ${RELEASE_FLAGS} main.c snake.c -o snake