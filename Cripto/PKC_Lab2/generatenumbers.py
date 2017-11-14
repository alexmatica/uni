import sys, random

if len(sys.argv) < 2:
	print 'Usage: generatenumbers.py <number_of_digits>'

for i in xrange(500):
	dig = int(sys.argv[1])
	a = random.randint(10**(dig-1), 10**dig - 1)
	b = random.randint(10**(dig-1), 10**dig - 1)
	print '%d,%d' % (a,b)