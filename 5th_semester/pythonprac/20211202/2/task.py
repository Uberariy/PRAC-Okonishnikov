def check_annotations(C: object) -> bool:
	return all([hasattr(C, i) and isinstance((getattr(C, i)), C.__annotations__[i]) for i in C.__annotations__])

class check(type):
	def __init__(self, name, parents, ns):
		setattr(self, "check_annotations", check_annotations)
		return super().__init__(name, parents, ns)

import sys
exec(sys.stdin.read())

