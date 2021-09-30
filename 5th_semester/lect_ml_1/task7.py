def process(l):
    return sorted(set(d*d for k in l for d in k))[::-1]