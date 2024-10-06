#!/usr/bin/env python3
import os

import matplotlib.pyplot as plt
import numpy

SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))
WINDOW_LENGTHS = range(1, 6)

plt.rcParams["font.family"] = "monospace"

with open(os.path.join(SCRIPT_DIR, "usedInstructions.txt"), "r") as f:
    used_instructions = f.read().splitlines()

for window_length in WINDOW_LENGTHS:
    window_frequencies = {}

    for window_end in range(window_length, len(used_instructions) + 1):
        window = "\n".join(used_instructions[window_end - window_length : window_end])
        if window in window_frequencies:
            window_frequencies[window] += 1
        else:
            window_frequencies[window] = 1

    windows = sorted([x[::-1] for x in window_frequencies.items() if x[1] > 1])

    y_bars = numpy.arange(len(windows))
    plt.barh(numpy.arange(len(windows)), [window[0] for window in windows])

    plt.title("Instruction windows frequency")
    plt.xlabel("Occurrences")
    plt.ylabel("Instruction windows")
    plt.gca().set_xticks(list(sorted(set(window[0] for window in windows))))
    plt.gca().set_yticks(y_bars, labels=[window[1] for window in windows])
    plt.tick_params(left=False, labeltop=True)
    plt.gca().xaxis.grid(True)

    plt.gcf().set_size_inches(5, len(windows) * window_length / 3)
    plt.savefig(
        os.path.join(SCRIPT_DIR, f"instructionWindows{window_length}.png"),
        bbox_inches="tight",
        dpi=300,
    )
    plt.close()
