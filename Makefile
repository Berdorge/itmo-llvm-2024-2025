CC=clang

SDL_CFLAGS=-lSDL2
SDL_SOURCES=$(wildcard SDL/*.c)
SDL_OUTPUT=SDL/sdl.out

PASS_SOURCES=LLVM_Pass/pass.cpp
PASS_LOGGER_SOURCES=LLVM_Pass/logger.cpp
PASS_INCLUDE=$(shell llvm-config --includedir)
PASS_OUTPUT=LLVM_Pass/libPass.so
PASS_LOGGER_OUTPUT=LLVM_Pass/logger.o

SDL_WITH_PASS_OUTPUT=SDL/sdlWithPass.out

ifeq ($(SDL_ITERATION_LIMIT),)
	SDL_ITERATION_LIMIT_FLAG=
else
	SDL_ITERATION_LIMIT_FLAG=-DITERATION_LIMIT=$(SDL_ITERATION_LIMIT)
endif

$(SDL_OUTPUT): $(SDL_SOURCES)
	$(CC) $(SDL_SOURCES) -o $(SDL_OUTPUT) $(SDL_ITERATION_LIMIT_FLAG) $(SDL_CFLAGS) 

$(PASS_OUTPUT): $(PASS_SOURCES)
	clang++ $(PASS_SOURCES) -fPIC -shared -I$(PASS_INCLUDE) -o $(PASS_OUTPUT)

$(SDL_WITH_PASS_OUTPUT): $(PASS_OUTPUT) $(SDL_SOURCES)
	clang++ -c -o $(PASS_LOGGER_OUTPUT) $(PASS_LOGGER_SOURCES)
	clang -fpass-plugin=$(PASS_OUTPUT) \
		-lstdc++ \
		-O2 \
		$(SDL_ITERATION_LIMIT_FLAG) \
		-o $(SDL_WITH_PASS_OUTPUT) \
		$(SDL_CFLAGS) \
		$(PASS_LOGGER_OUTPUT) $(SDL_SOURCES)

.PHONY: all sdl run-sdl pass sdl-with-pass run-sdl-with-pass analyze-sdl clean

all: $(SDL_OUTPUT) $(SDL_WITH_PASS_OUTPUT)

sdl: $(SDL_OUTPUT)

run-sdl: $(SDL_OUTPUT)
	$(SDL_OUTPUT)

pass: $(PASS_OUTPUT)

sdl-with-pass: $(SDL_WITH_PASS_OUTPUT)

run-sdl-with-pass: $(SDL_WITH_PASS_OUTPUT)
	$(SDL_WITH_PASS_OUTPUT)

analyze-sdl:
	$(MAKE) SDL_ITERATION_LIMIT=10 clean run-sdl-with-pass
	SDL/stats/analyze.py
	@echo "You may now find instruction windows analysis in SDL/stats directory."

clean:
	rm -f $(SDL_OUTPUT) \
		$(PASS_OUTPUT) \
	 	$(PASS_LOGGER_OUTPUT) \
		$(SDL_WITH_PASS_OUTPUT)
