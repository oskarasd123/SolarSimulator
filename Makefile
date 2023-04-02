all: src/main.c
	gcc src/main.c -O0 -ggdb -std=c99 -Wall -Wextra -pedantic -lGL -lm -lGLEW -lglfw -o main