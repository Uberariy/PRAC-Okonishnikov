class Num:
	val = 0
	def __get__(self, obj, cls):
		return getattr(obj, "val", 0)
	def __set__(self, obj, value):
		try:
			value.real
			obj.val = value
		except Exception:
			obj.val = len(value)
		return
	def __delete__(self, obj):
		print("del")

import sys
exec(sys.stdin.read())

