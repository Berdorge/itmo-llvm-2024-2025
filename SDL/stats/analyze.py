#!/usr/bin/env python3
import os

SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))
WINDOW_LENGTHS = range(1, 6)

with open(os.path.join(SCRIPT_DIR, "usedInstructions.txt"), "r") as f:
    used_instructions = f.read().splitlines()

for window_length in WINDOW_LENGTHS:
    window_frequencies = {}

    for window_end in range(window_length, len(used_instructions) + 1):
        window = "\n".join(
            used_instructions[window_end - window_length : window_end]
        )
        if window in window_frequencies:
            window_frequencies[window] += 1
        else:
            window_frequencies[window] = 1

    windows = sorted([x[::-1] for x in window_frequencies.items()], reverse=True)

    with open(
        os.path.join(SCRIPT_DIR, f"instructionWindows{window_length}.txt"), "w"
    ) as f:
        f.write(
            f"Instruction windows of length {window_length}, "
            "from most frequent to least frequent:\n\n"
        )
        for occurrences, window in windows:
            f.write(f"This window occurred {occurrences} time(s):\n")
            f.write(window)
            f.write("\n\n")
