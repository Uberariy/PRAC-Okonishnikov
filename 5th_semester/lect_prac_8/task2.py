class Triangle():
    a=b=c=0
    abss=0

    def __init__(self, a,b,c):
        self.a=float(a);   self.b=float(b);   self.c=float(c)
        if a<=0 or b<=0 or c<=0 or max(a,b,c)>=sum([a,b,c])-max(a,b,c):
            pass
        else:
            p = (self.a+self.b+self.c)/2
            self.abss = (p*(p-self.a)*(p-self.b)*(p-self.c))**0.5
    def __abs__(self):
        return self.abss
    def __str__(self):
        return "{}:{}:{}".format(self.a,self.b,self.c)
    def __bool__(self):
        return bool(self.abss)

    def __eq__(self, other):
        return self.abss == other.abss
    def __lt__(self, other):
        return self.abss < other.abss
    def __le__(self, other):
        return self.abss <= other.abss
    def __gt__(self, other):
        return self.abss > other.abss
    def __ge__(self, other):
        return self.abss >= other.abss


import sys
exec(sys.stdin.read())