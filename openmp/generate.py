#!/usr/bin/python

import random

N = 4000 

matrix = open('matrix4000', 'wb')
for i in range(1, N+1):
	for j in range(1 , N+1):
		row = '%d %d %d\n' % (i, j, random.randint(0,99))
		matrix.write(row)
