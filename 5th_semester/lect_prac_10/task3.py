from functools import wraps
def counter(fn):
    def cout():
        return fun.count
    @wraps(fn)
    def fun(*args, **kwargs):
        fun.count += 1
        return fn(*args, **kwargs)
    fun.count = 0
    fun.counter = cout
    return fun	
    
import sys
exec(sys.stdin.read())