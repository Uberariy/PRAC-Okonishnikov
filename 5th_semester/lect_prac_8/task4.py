from fractions import *
import fractions
class Sausage():
    farsh = "pork!"
    volu = 12

    def __init__(self, f="pork!", v="1/1"):
        self.farsh = (f*12)[:12]; 
        #print(f, v, (Fraction(v)*12))    
        self.volu = Fraction(v)
        if self.volu < 0:
            self.volu = Fraction(0)

    def __str__(self):
        a = b = c = ""
        #print(self.volu, int(self.volu))
        for n in range(0, int(self.volu*12), 12):
            a += "/------------\\"
            b += "|"+self.farsh+"|"
            c += "\\------------/"
        if a=="":
            a += "/|"
            b += "||"
            c += "\\|"
        elif int(self.volu*12)%12!=0:
            a = a[:-13+int(self.volu*12)%12]+"|"
            b = b[:-13+int(self.volu*12)%12]+"|"
            c = c[:-13+int(self.volu*12)%12]+"|"
        return a+"\n"+(b+"\n")*3+c

    def __bool__(self):
        return bool(self.volu)
    def __add__(self, other):
        #print(self.volu + other.volu, self.volu, other.volu)
        return Sausage(self.farsh, (self.volu+other.volu))
    def __sub__(self, other):
        return Sausage(self.farsh, (self.volu-other.volu))
    def __mul__(self, other):
        return Sausage(self.farsh, self.volu*other)
    def __truediv__(self, other):
        return Sausage(self.farsh, self.volu/(other))
    def __abs__(self):
        return self.volu if self.volu>0 else 0
    __rmul__ = __mul__


import sys
exec(sys.stdin.read())