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

async def joiner():
    n = 0; tasks = []
    E = collections.defaultdict(asyncio.Event)
    for p in range(4):
        b = 2**(p+1)
        for i in range(0, len(L), b):
            tasks.append(asyncio.create_task(merge(i, i+b//2, i+b, E[f"{p}_{i}"], E[f"{p}_{i+b}"], E[f"{p}_{i+b//2}"])))
            n += 1
    for i in E:
        E[i].set()
    await asyncio.gather(*tasks)

L = list(eval(input()))
LL = L.copy()
asyncio.run(joiner())
print(L)