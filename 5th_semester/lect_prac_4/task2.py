def BinPow(a, N, f):
    if N == 1:
        return a
    elif N % 2 == 1:
        return f(BinPow(a, N-1, f), a)
    else:
        b = BinPow(a, N//2, f)
        return f(b, b)

