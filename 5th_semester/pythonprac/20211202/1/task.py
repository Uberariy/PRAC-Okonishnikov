from functools import wraps
class dump(type):
    
    def __init__(self, name, parents, ns):
        def wrapper(fn, name):
            @wraps(fn)
            def fun(*args, **kwargs):
                print("{}: {}, {}".format(name,  args[1:], kwargs))
                return fn(*args, **kwargs)
            return fun

        for i in ns:
            if callable(ns[i]):
                setattr(self, i, wrapper(ns[i], i))
        return super().__init__(name, parents, ns)

import sys
exec(sys.stdin.read())

