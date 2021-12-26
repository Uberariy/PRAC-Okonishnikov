class MyClass(BaseException):
    pass
class Tester():
    fun=0

    def __init__(self, fun):
        self.fun = fun

    def __call__(self, suite=[], allowed=[MyClass]):
        res = 0
        for i in suite:
            try:
                self.fun(*i)
            except tuple(allowed):
                res = -1
            except Exception:
                res = 1
        return res

import sys
exec(sys.stdin.read())