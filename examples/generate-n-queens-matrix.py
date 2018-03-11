#!/usr/bin/env python

"""
Generate exact cover matrices for the generalized n-queens problem.
"""

import sys

if len(sys.argv) < 2:
    sys.exit('Usage: %s n' % sys.argv[0])

n = int(sys.argv[1])


pcols = 2 * n
scols = 4 * n - 2

# Inform the solver of the primary / secondary column count.
print (pcols, scols)

for r in range(n): 
  for c in range(n):
    rc_r = r
    rc_c = c
    rc_tld = c + r
    rc_trd = n - 1 - c + r

    row = ['0'] * (scols + pcols)
    for i in [rc_r, n + rc_c, 2 * n + rc_tld, 4 * n - 1 + rc_trd]:
      row[i] = '1'
    print ''.join(row) 
