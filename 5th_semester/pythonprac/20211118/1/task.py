def objcount(cls):
	class declass(cls):
		counter = 0
		def __init__(self, *args, **kwargs):
			try:
				super().__init__(*args, **kwargs)
			except Exception:
				super().__init__()
			declass.counter += 1
	
		def __del__(self):
			if getattr(super(), "__del__", None) is not None:
				super().__del__()
			declass.counter -= 1
	return declass

import sys
exec(sys.stdin.read())


