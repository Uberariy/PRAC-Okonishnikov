def superposition(funmod, funseq):
    L = []
    for i in funseq:
        def f(x, j=i):
            return funmod(j(x))
        L.append(f)
    return L
