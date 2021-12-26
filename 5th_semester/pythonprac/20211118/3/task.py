import string
class Alpha:
	__slots__ = [*string.ascii_lowercase]
	
	def __str__(self):
		return ", ".join([str(i)+": "+str(getattr(self, i, None)) for i in string.ascii_lowercase if getattr(self, i, None) != None])
			
	def __init__(self, **kwargs):
		for i, j in kwargs.items():
			setattr(self, i, j)

class DDF943PDS4321:
	def __init__(self, *args, **kwargs):
		pass
	pass	
class AlphaQ(DDF943PDS4321):
	def __str__(self):
		return ", ".join([str(i)+": "+str(getattr(self, i, None)) for i in string.ascii_lowercase if getattr(self, i, None) != None])
			
	def __init__(self, **kwargs):
		for i, j in kwargs.items():
			if i in string.ascii_lowercase:
				setattr(self, i, j)
			else:
				self.i # Raise AttributeError

	def __setattr__(self, obj, val):
		if obj in [*string.ascii_lowercase]+["obj"]:
			if getattr(self, obj, None) != val:
				super(DDF943PDS4321, self).__setattr__(obj, val)
		else:
			self.obj # Raise AttributeError

import sys
exec(sys.stdin.read())
