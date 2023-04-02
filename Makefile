all: src/main.c
	gcc src/main.c -O2 -std=c99 -Wall -Wextra -pedantic -lGL -lm -lGLEW -lglfw -o main