import sys
l = sys.stdin.buffer.read()
N = l[0]
o = l[0:1]
l = l[1:]

s = sorted([l[i*len(l)//N:(i+1)*len(l)//N] for i in range(0, N, 1)])

sys.stdout.buffer.write(o)
for i in s:
	sys.stdout.buffer.write(i)



