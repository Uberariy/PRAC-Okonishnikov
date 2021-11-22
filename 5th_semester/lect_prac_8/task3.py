class Dots():
    a=b=float(0)

    def __init__(self, a, b):
        self.a=float(a);   self.b=float(b);
    def __getitem__(self, item):
        if type(item)==type(1):
            return([self.a+(self.b-self.a)/(item-1)*i for i in range(0, item)])
        if type(item)==type(slice(1, 2)):
            t = [item.start if item.start != None else 0, item.stop if item.stop != None else item.step, item.step]
            if item.step == None:
                return self.a+(self.b-self.a)/(t[1]-1)*t[0]
            if type(item)==type(slice(1, 2)):
                def gen(t):
                    for i in range(t[0], t[1]):
                        yield self.a+(self.b-self.a)/(t[2]-1)*i
                return gen(t)


import sys
exec(sys.stdin.read())