#!/usr/bin/env python

"""
Generate exact cover matrices to find latin squares of size n.

For every combination of row, column, value, we include 3 constraints:
1) value occupies row (r_v)
2) value occupies column (c_v)
3) value occupies cell (r_c)

Note: the number of solutions rises quickly with n.  Here is a small table:
n solutions
1 1
2 2
3 12
4 576
5 161280
6 812851200

See also: https://oeis.org/A002860/list
"""

import sys

if len(sys.argv) < 2:
    sys.exit('Usage: %s n' % sys.argv[0])

n = int(sys.argv[1])

# Inform the solver of the primary / secondary column count.
print (3 * n * n, 0)

for r in range(n): 
  for c in range(n):
    for v in range(n):
      r_v = r * n + v
      c_v = c * n + v
      r_c = r * n + c
      row = ['0'] * 3 * n * n
      for i in [r_v, n * n + c_v, 2 * n * n + r_c]:
        row[i] = '1'
      print ''.join(row) 

