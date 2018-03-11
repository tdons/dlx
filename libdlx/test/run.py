#!/usr/bin/env python

import os
import sys

pydlx_dir = '../..'
sys.path.insert(1, os.path.join(sys.path[0], pydlx_dir))

from pydlx.dlx_matrix import dlx_matrix
from pydlx.dlx_iterative_solver import dlx_iterative_solver

lines = open(sys.argv[1]).read().split('\n')

# First line contains metadata about the division between primary and
# secondary columns.
metadata = lines[0]
(num_primary_columns, num_secondary_columns) = eval(metadata)

# Find the line reading "solutions"
solution_row = lines.index("solutions")

# Extract the matrix and the solutions from the input file
matrix = lines[1:solution_row]
solutions = [s for s in lines[solution_row + 1:] if len(s) > 0]


# Convert the input rows to lists of indices of where the 1's occur
# For instance:
# ['001', '101', '010'] -> [[2], [0, 2], [1]]
matrix = [[i for i, x in enumerate(row) if x == '1'] for row in matrix]

# Convert [['1,2']] -> [set([1, 2])]
solutions = set([frozenset(int(i) for i in s.split(',')) for s in solutions])

# Compute the parameters for dlx_matrix_create
num_nodes = 0
if len(matrix) > 0:
	num_nodes = sum([len(row) for row in matrix])

# Construct & solve the matrix.
with dlx_matrix(num_primary_columns, num_secondary_columns, num_nodes) as mat:
	for i, row in enumerate(matrix):
		mat.add_row(row, i)

	with dlx_iterative_solver(mat) as solver:
		computed_solutions = set()
		while True:
			sol = solver.get_next_solution()
			if not sol:
				break
			computed_solutions.add(frozenset(sol))

if solutions != computed_solutions:
	print 'got', computed_solutions
	print 'expected', solutions
	sys.exit(1)

sys.exit(0)
