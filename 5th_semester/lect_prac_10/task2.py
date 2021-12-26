#from functools import wraps
def sizer(cls):
    #@wraps(dict)
    class declass(cls):
        args = 0
        def __init__(self, *args, **kwargs):
            res = cls(*args, **kwargs)
           # print(res, type(res))
            self.args = res
            return

        def __repr__(self):
            return str(self.args)

        def __getattr__(self, a):
            try:
                return len(self.args)
            except Exception:
                try:
                    return abs(self.args)
                except Exception:
                    return 0
    return declass
    
import sys
exec(sys.stdin.read())