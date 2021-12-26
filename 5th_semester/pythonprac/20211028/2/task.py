import itertools
def slide(seq, n):
	i = 0
	a, b = itertools.tee(itertools.islice(seq, i, i+n))
	while [j for j in a]:
		yield from b
		i += 1
		a, b = itertools.tee(itertools.islice(seq, i, i+n))

import sys
exec(sys.stdin.read())
