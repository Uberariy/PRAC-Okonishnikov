class OrderedComplex(complex):
	def __lt__(self, other):
		return bool(1>0)
		

#import sys
#exec(sys.stdin.read())
a = OrderedComplex(1, 2)
dir(a)

