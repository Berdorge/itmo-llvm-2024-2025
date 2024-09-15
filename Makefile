CC=clang
SDL_CFLAGS=-lSDL2
SDL_SOURCES=$(wildcard SDL/*.c)
SDL_OUTPUT=SDL/sdl.out

all: sdl

sdl:
	$(CC) $(SDL_SOURCES) -o $(SDL_OUTPUT) $(SDL_CFLAGS) 

run-sdl: sdl
	$(SDL_OUTPUT)

.PHONY: clean

clean:
	rm $(SDL_OUTPUT)
