from decimal import *

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

def siin(x):
    getcontext().prec += 2
    i, endcasee, res, fact, num, sign = 1, 0, x, 1, x, 1
    while res != endcasee:
        endcasee = res
        i += 2
        fact *= i*(i-1)
        num *= x*x
        sign *= -1
        res += num / fact*sign
    getcontext().prec -= 2
    return +res

def coos(x):
    getcontext().prec += 2
    i, endcasee, res, fact, num, sign = 0, 0, 1, 1, 1, 1
    while res != endcasee:
        endcasee = res
        i += 2
        fact *= i*(i-1)
        num *= x*x
        sign *= -1
        res += num / fact*sign
    getcontext().prec -= 2
    return +res

N = 1500
x = int(input())
pr = int(input())
getcontext().prec = N
x *= pii()/Decimal("200")
mytan = siin(x)
mytan *= 1/coos(x)
getcontext().prec = pr
#print("0.5703899296732948869842944367165171344395916612277803588670767321648773185785139182667281174457178562297893243008142072872853286888960328100690865086908251916306044433718985107873049499896410747651198277527226796299574297144599467662451565824991733541829662982017242298421333390256456774571988988757135105640889382555549825129116125791099733839132220781770885074880665542087613574365108673861349851358")
print(+mytan)
#print(str(mytan)[:pr+1])


