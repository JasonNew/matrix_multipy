#!/usr/bin/python

import sys

N = 100

#matrix =open('matrix', 'r')
#for line in matrix:
for line in sys.stdin:
	line = line.strip()

	row, col, value = line.split()

	for colnum in range(1,N+1):
		print "%s,%s\ta,%s,%s" %  (row, colnum, col, value)
		print "%s,%s\tb,%s,%s" %  (colnum, col, row, value)
