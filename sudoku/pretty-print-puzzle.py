#!/usr/bin/env python

#
# Quick 'n dirty pretty print script
#
# Reads lines from stdin, each line should be a 81 digit character string
# representing a sudoku puzzle.  Output is a pretty printed version of the
# puzzle.
#

import sys

for p in sys.stdin:
	for r in range(9):
		if r % 3 == 0:
			print '+---+---+---+'
		for c in range(9):
			if c % 3 == 0:
				sys.stdout.write('|')
			v = p[r * 9 + c]
			if v == '0':
				v = ' '
			sys.stdout.write(v)
		sys.stdout.write('|')
		print
	print '+---+---+---+'
