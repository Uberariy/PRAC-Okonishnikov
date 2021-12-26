class DivStr(str):

    def __init__(self, fun):
        self.fun = fun

    def __floordiv__(self, other):
        N = len(self)//int(other)
        if N != 0:
            return [self[i:i+N] for i in range(0,int(other)*N,N)]
        else:
            return [""]*int(other)
    
    def __mod__(self, other):
        N = len(self)%int(other)
        return DivStr(self[::-1][0:N][::-1])

    def wrapper(fun, name):
        def wrapins(*args, **kwargs):
            res = fun(*args, **kwargs)
            if type(res) == type("fd"):
                return DivStr(res)
            else:
                return res
        wrapins.__name__ = name
        return wrapins

    for i in str.__dict__:
        if str(i)[0:2]!="__":
            locals()[i] = wrapper(str.__dict__[i], i)
        locals()["__mul__"] = wrapper(str.__dict__["__mul__"], i)
        locals()["__rmul__"] = wrapper(str.__dict__["__rmul__"], i)
        locals()["__add__"] = wrapper(str.__dict__["__add__"], i)
        locals()["__getitem__"] = wrapper(str.__dict__["__getitem__"], i)
    
import sys
exec(sys.stdin.read())