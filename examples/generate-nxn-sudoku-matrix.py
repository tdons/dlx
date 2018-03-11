#!/usr/bin/env python

"""
Generate exact cover matrices to find sudoku grids with box-size n.

For every combination of row, column, value, we include 4 constraints:
1) value occupies row (r_v)
2) value occupies column (c_v)
3) value occupies cell (r_c)
4) value occupies a box (b_v)

Note: the number of solutions rises *extremely* quickly with n.  The computation
      makes sense only for n = 2 and is infeasible for n = 3
"""

import sys

if len(sys.argv) < 2:
    sys.exit('Usage: %s n' % sys.argv[0])

bs = int(sys.argv[1])

# boxsize n yields a grid of size n * n
n = bs * bs

# Inform the solver of the primary / secondary column count.
print (4 * n * n, 0)

for r in range(n): 
  for c in range(n):
    for v in range(n):
      r_v = r * n + v
      c_v = c * n + v
      r_c = r * n + c
      b_v = (r / bs * bs + c / bs) * n + v
      row = ['0'] * 4 * n * n
      for i in [r_v, n * n + c_v, 2 * n * n + r_c, 3 * n * n + b_v]:
        row[i] = '1'
      print ''.join(row) 

