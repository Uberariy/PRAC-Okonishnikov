from functools import wraps
def cast(typ):
    def dump(fn):
        @wraps(fn)
        def fun(*args, **kwargs):
            return typ(fn(*args, **kwargs))
        return fun	
    return dump
    
import sys
exec(sys.stdin.read())