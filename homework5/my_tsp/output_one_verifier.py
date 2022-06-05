#!/usr/bin/env python3

import math

# copied from common.py
def read_input(filename):
    with open(filename) as f:
        cities = []
        for line in f.readlines()[1:]:  # Ignore the first line.
            xy = line.split(',')
            cities.append((float(xy[0]), float(xy[1])))
        return cities


def distance(city1, city2):
    return math.sqrt((city1[0] - city2[0]) ** 2 + (city1[1] - city2[1]) ** 2)


def verify_output():
    print('input Challenge_number >>',end='')
    challenge_number = int(input())
    print(f'Challenge {challenge_number}')
    cities = read_input(f'inputs/input_{challenge_number}.csv')
    N = len(cities)

    # change here to generate your result
    print(f'input result file name\n(format: {{name}}_{challenge_number}.csv)\n>>',end='')
    output_prefix = str(input())
    output_file = f'results/{output_prefix}_{challenge_number}.csv'
    with open(output_file) as f:
        lines = f.readlines()
        assert lines[0].strip() == 'index'
        tour = [int(i.strip()) for i in lines[1:N + 1]]
    assert set(tour) == set(range(N))
    path_length = sum(distance(cities[tour[i]], cities[tour[(i + 1) % N]])
                      for i in range(N))
    print(f'{output_prefix:16}: {path_length:>10.2f}')
    print()


if __name__ == '__main__':
    verify_output()
