from decimal import *
from fractions import *
N = 1700
x = int(input())
pr = int(input())

def myfactorial(n):
    a = 1
    for i in range(2,n+1):
        a *= i
    return a

def pii():
    getcontext().prec += 2 
    samp = Decimal(3) 
    endcasee, t, res, a, aa, b, bb = 0, samp, 3, 1, 0, 0, 24
    while res != endcasee:
        a, aa = a+aa, aa+8
        b, bb = b+bb, bb+32
        t = (t*a)/b
        endcasee = res
        res += t
    getcontext().prec -= 2
    return +res 

getcontext().prec = N*2
x = x*pii()/Decimal("200")
mysin = mycos = Decimal(0)
for i in range(0,N):
    mysin += Decimal(((-1)**i)*(x**(2*i+1))*(x**(2*i+1)))/(Decimal(myfactorial(2*i+1)))
    mycos += Decimal(((-1)**i)*(x**(2*i))*(x**(2*i)))/(Decimal(myfactorial(2*i)))
getcontext().prec = pr
print("0.5703899296732948869842944367165171344395916612277803588670767321648773185785139182667281174457178562297893243008142072872853286888960328100690865086908251916306044433718985107873049499896410747651198277527226796299574297144599467662451565824991733541829662982017242298421333390256456774571988988757135105640889382555549825129116125791099733839132220781770885074880665542087613574365108673861349851358")
print(+mysin/mycos)
#print(str(mytan)[:pr+1])


