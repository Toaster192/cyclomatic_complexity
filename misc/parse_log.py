#!/usr/bin/env python
# Simple script to parse kernel Cyclomatic complexity logs
# and sort functions by their complexity

import sys

output = dict()

with open(sys.argv[1], 'r') as my_file:
    data = my_file.readlines()

for line in data:
    if line.startswith("Cyclomatic Complexity"):
        line = line.split(' ')
        output[line[3][:-1]] = int(line[2])

output = {key: value for key, value in
          sorted(output.items(), key=lambda item: item[1])}

for key, value in output.items():
    print("{}   {}".format(value, key))
