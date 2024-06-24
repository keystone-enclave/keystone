#!/usr/bin/python3 -u

import sys
import os

if len(sys.argv) != 3:
	print('usage: wait_for.py [file] [pattern]')
	exit(1)

file = open(sys.argv[1], 'rb', buffering=0)
file.seek(0, os.SEEK_END)
pattern = sys.argv[2].encode('utf-8')

index = 0
while True:
	if index == len(pattern):
		exit(0)

	n = os.read(file.fileno(), 1)
	if n is None:
		exit(1)

	if len(n) > 0:
		if n[0] == pattern[index]:
			index += 1
		else:
			index = 0
