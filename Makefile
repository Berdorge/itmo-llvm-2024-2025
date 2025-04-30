SDL_CFLAGS=-lSDL2
SDL_SOURCES=$(wildcard SDL/*.c)
SDL_SIM_SOURCES=SDL/sim.c
SDL_SOURCES_WITHOUT_APP=$(filter-out SDL/app.c, $(SDL_SOURCES))
SDL_OUTPUT=SDL/sdl.out

PASS_SOURCES=LLVM_Pass/pass.cpp
PASS_LOGGER_SOURCES=LLVM_Pass/logger.cpp
PASS_INCLUDE=$(shell llvm-config --includedir)
PASS_OUTPUT=LLVM_Pass/libPass.so
PASS_LOGGER_OUTPUT=LLVM_Pass/logger.o

SDL_WITH_PASS_OUTPUT=SDL/sdlWithPass.out

GENERATOR_SOURCES=SDL/IRGen/sdlAppGenerator.cpp
GENERATOR_OUTPUT=SDL/IRGen/sdlAppGenerator.out
SDL_GENERATED_SOURCES=SDL/appGenerated.ll
SDL_GENERATED_OUTPUT=SDL/sdlGenerated.out

ASM_SOURCES=SDL/IRGen/app.s
EMULATED_ASM_IRGEN_SOURCES=SDL/IRGen/emulatedAsmIRGen.cpp
EMULATED_ASM_IRGEN_OUTPUT=SDL/IRGen/emulatedAsmIRGen.out
ASM_IRGEN_SOURCES=SDL/IRGen/asmIRGen.cpp
ASM_IRGEN_OUTPUT=SDL/IRGen/asmIRGen.out

ifeq ($(SDL_ITERATION_LIMIT),)
	SDL_ITERATION_LIMIT_FLAG=
else
	SDL_ITERATION_LIMIT_FLAG=-DITERATION_LIMIT=$(SDL_ITERATION_LIMIT)
endif

$(SDL_OUTPUT): $(SDL_SOURCES)
	clang $(SDL_SOURCES) -o $(SDL_OUTPUT) $(SDL_ITERATION_LIMIT_FLAG) $(SDL_CFLAGS) 

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

$(GENERATOR_OUTPUT): $(SDL_SIM_SOURCES) $(GENERATOR_SOURCES)
	clang++ $(shell llvm-config --cppflags --ldflags --libs) \
		$(SDL_SIM_SOURCES) $(GENERATOR_SOURCES) \
		$(SDL_CFLAGS) \
		-o $(GENERATOR_OUTPUT)

$(SDL_GENERATED_SOURCES): $(GENERATOR_OUTPUT)
	$(GENERATOR_OUTPUT) $(SDL_GENERATED_SOURCES)

$(SDL_GENERATED_OUTPUT): $(SDL_SOURCES_WITHOUT_APP) $(SDL_GENERATED_SOURCES)
	clang $(SDL_SOURCES_WITHOUT_APP) $(SDL_GENERATED_SOURCES) \
		-o $(SDL_GENERATED_OUTPUT) \
		$(SDL_ITERATION_LIMIT_FLAG) \
		$(SDL_CFLAGS) 

$(EMULATED_ASM_IRGEN_OUTPUT): $(EMULATED_ASM_IRGEN_SOURCES) $(ASM_SOURCES)
	clang++ --std=c++20 -g -O0 $(shell llvm-config --cppflags --ldflags --libs) \
		$(EMULATED_ASM_IRGEN_SOURCES) $(SDL_SIM_SOURCES) \
		$(SDL_CFLAGS) \
		-o $(EMULATED_ASM_IRGEN_OUTPUT)

$(ASM_IRGEN_OUTPUT): $(ASM_IRGEN_SOURCES) $(ASM_SOURCES)
	clang++ --std=c++20 -g -O0 $(shell llvm-config --cppflags --ldflags --libs) \
		$(ASM_IRGEN_SOURCES) $(SDL_SIM_SOURCES) \
		$(SDL_CFLAGS) \
		-o $(ASM_IRGEN_OUTPUT)

.PHONY: all
.PHONY: sdl run-sdl
.PHONY: pass sdl-with-pass run-sdl-with-pass analyze-sdl
.PHONY: generator run-generator generated-sdl run-generated-sdl run-interpreted-sdl
.PHONY: emulated-asm run-emulated-asm
.PHONY: asm run-asm
.PHONY: clean

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

generator: $(GENERATOR_OUTPUT)

run-generator: $(SDL_GENERATED_SOURCES)

generated-sdl: $(SDL_GENERATED_OUTPUT)

run-generated-sdl: $(SDL_GENERATED_OUTPUT)
	$(SDL_GENERATED_OUTPUT)

run-interpreted-sdl: $(GENERATOR_OUTPUT)
	$(GENERATOR_OUTPUT)

emulated-asm: $(EMULATED_ASM_IRGEN_OUTPUT)

run-emulated-asm: $(EMULATED_ASM_IRGEN_OUTPUT)
	$(EMULATED_ASM_IRGEN_OUTPUT) $(ASM_SOURCES)

asm: $(ASM_IRGEN_OUTPUT)

run-asm: $(ASM_IRGEN_OUTPUT)
	$(ASM_IRGEN_OUTPUT) $(ASM_SOURCES)

clean:
	rm -f $(SDL_OUTPUT) \
		$(PASS_OUTPUT) \
	 	$(PASS_LOGGER_OUTPUT) \
		$(SDL_WITH_PASS_OUTPUT) \
		$(GENERATOR_OUTPUT) \
		$(SDL_GENERATED_SOURCES) \
		$(SDL_GENERATED_OUTPUT) \
		$(EMULATED_ASM_IRGEN_OUTPUT) \
		$(ASM_IRGEN_OUTPUT) \
