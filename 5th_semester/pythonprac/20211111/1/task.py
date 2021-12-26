class OrderedComplex(complex):
	def __lt__(self, other):
		return bool(self.imag < other.imag and self.real < other.real)
		
class OrderedComplexMul(OrderedComplex):
	def __matmul__(self,other):
		return float(self.real*other.real+self.imag*other.imag)

import sys
exec(sys.stdin.read())


