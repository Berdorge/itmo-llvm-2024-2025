## Conway's game of life simulation through SDL graphical app
In order to build and launch the graphical app with
Conway's Game of Life simulation, run
```sh
make run-sdl
```

## SDL graphical app instruction windows frequency analysis
In order to analyze the SDL graphical app
instruction windows frequencies
through LLVM instrumented pass,
run
```sh
make analyze-sdl
```
Be aware that the script
(located at `./SDL/stats/analyze.py`)
used to plot instruction windows frequencies
inside this `make` rule
requires
[`matplotlib`](https://matplotlib.org/) to be installed.

## Compiled SDL graphical app with generated sources
In order to generate the LLVM IR of
the SDL graphical app, then compile and launch it,
run
```sh
make run-generated-sdl
```

## Interpreted SDL graphical app with generated sources
In order to generate the LLVM IR of
the SDL graphical app (without dumping it anywhere),
then interpret it,
run
```sh
make run-interpreted-sdl
```

## Interpreted SDL graphical app with sources generated from SARCH assembly
In order to generate the LLVM IR of
the SDL graphical app from SARCH assembly sources,
then interpret it,
run
```sh
make run-emulated-asm
```
or
```sh
make run-asm
```
The former generates bytecode with all SARCH instructions emulated,
while the latter generates direct IR instructions and
emulates only register access.
