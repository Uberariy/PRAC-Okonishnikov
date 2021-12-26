import math
import random
import asyncio
import collections

async def merge(b0, b1, c1, eL, eR, eCurr):
    if b1-b0 == 0:
        await asyncio.sleep(0)
    else:
        await eL.wait()
        await eR.wait()
    b, c0, i = b0, b1, b0
    while b0 < c0 and b1 < c1:
        if L[b0] < L[b1]:
            LL[i] = L[b0]
            b0 += 1
        else:
            LL[i] = L[b1]
            b1 += 1
        i += 1
    LL[i:c1] = L[b0:c0] + L[b1:c1]
    L[b:c1] = LL[b:c1]
    eCurr.set()

async def joiner(N):
    n = 0; tasks = []
    E = collections.defaultdict(asyncio.Event)
    it = math.ceil(math.log2(N))
    for p in range(it):
        b = 2**(p+1)
        for i in range(0, N, b):
            tasks.append(asyncio.create_task(merge(i, min(N, i+b//2), min(N, i+b), E[f"{min(N, p)}_{min(N, i)}"], E[f"{min(N, p)}_{min(N, i+b)}"], E[f"{min(N, p)}_{min(N, i+b//2)}"])))
            n += 1
    for i in E:
        E[i].set()
    await asyncio.gather(*tasks)

L = list(eval(input()))
LL = L.copy()
N = len(L)
asyncio.run(joiner(N))
print(L)