## Conway's game of life simulation through SDL graphical app

To build and launch the graphical app with
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
