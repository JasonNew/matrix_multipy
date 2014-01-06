#!/usr/bin/python

import sys

#matrix = open('mid','r')

current_key = None

N = 1000
ma = [0]*N
mb = [0]*N
sum = 0
#for line in matrix:
for line in sys.stdin:
    line = line.strip()

    key, value = line.split('\t')
    
    if current_key == key:
        value = value.split(',')
        if value[0]=='a':
            ma[int(value[1])-1] = int(value[2])
        elif value[0]=='b':
            mb[int(value[1])-1] = int(value[2])
    else:
        for i in range(0,N):
            sum = sum + ma[i]*mb[i]
        if current_key:
            print '%s\t%s' % (current_key, sum)
        current_key = key
        ma = [0]*N
        mb = [0]*N
        sum = 0
        value = value.split(',')
        if value[0]=='a':
            ma[int(value[1])-1] = int(value[2])
        elif value[0]=='b':
            mb[int(value[1])-1] = int(value[2])

for i in range(0,N):
    sum = sum + ma[i]*mb[i]
print '%s\t%s' % (current_key, sum)
