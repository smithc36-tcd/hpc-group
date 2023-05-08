import numpy as np
import sys
import re
# import matplotlib.pyplot as plt


def printRes(filename):
    '''Calculate the latency and bandwidth of the ping-pong results'''
    prefix = filename[0:5]

    with open(filename, 'r') as f:
        content = f.read()
        content = re.sub(r'[\t ]+', '', content)
        content = re.sub(r'0\.', ' 0.', content)

    # Read column 1 into x and column 2 into y
    x, y = np.loadtxt(content.splitlines(), delimiter=' ', unpack=True)
    p1, p2 = np.polyfit(x, y, 1)

    # Calculate using the slope and intercept from polyfit
    bandwidth = (1 / p1) / 1e9
    latency = p2 / 1e-6

    # print(f"{prefix}-communication results: {p1} {p2}")
    print("==========================================================")
    print(f"Bandwidth of {prefix}-communication:   {bandwidth} GB/s")
    print(f"Latency of {prefix}-communication:     {latency} us")
    print("==========================================================")
    print()


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python program_name.py input_file")
        sys.exit(1)

    filename = sys.argv[1]
    printRes(filename)
