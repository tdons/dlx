#!/usr/bin/env python
import time
import os
import sys

"""
This program counts the number of solutions to the exact cover problem
presented to it on stdin in the form of a binary matrix.

For instance:
$ echo -e "(2,0)\n01\n10" | ./count-solutions.py
1
"""

# make sure that both pydlx and libdlx can be found
pydlx_dir = '..'
sys.path.append(os.path.join(sys.path[0], pydlx_dir))

from pydlx.dlx_matrix import dlx_matrix
from pydlx.dlx_recursive_solver import count_solutions

# First read the metadata
metadata = sys.stdin.readline()
(num_primary_columns, num_secondary_columns) = eval(metadata)

# Read everything from stdin
matrix = sys.stdin.readlines()

# Create lists of indices of 1's for each row
# For instance:
# ['001', '101', '010'] -> [[2], [0, 2], [1]]
matrix = [[i for i, x in enumerate(row) if x == '1'] for row in matrix]

num_nodes = sum([len(row) for row in matrix])

# Solve the matrix
with dlx_matrix(num_primary_columns, num_secondary_columns, num_nodes) as mat:
	for i, row in enumerate(matrix):
		mat.add_row(row, i)

	print count_solutions(mat)
