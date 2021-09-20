from os import system
import sys

if __name__ == "__main__":
    if len (sys.argv) > 1:
        n = int(sys.argv[1])
    else:
        print("-> Could you please type power of the net, that would be DESTROYED?")
        print("==> ", end = '')
        n = int(input())

for i in range(1, n+1, 1):
    system("ip netns exec ns"+str(i)+" ip link set dev ns"+str(i)+"-sw"+str(i)+" netns "+str(i))
    system("ip netns del ns"+str(i))

edges = []
for i in range(1, n+1, 1):
    for j in range(i+1, n+1, 1):
        edges += ["sw"+str(i)+"-sw"+str(j)]
    edges += ["sw"+str(i)+"-ns"+str(i)]

swchs = []
for i in range(1, n+1, 1):
    swchs += ["sw"+str(i)]

for edge in edges:
	system("ip link del "+edge)

for swch in swchs:
    system("ovs-vsctl del-br "+swch)

print("-> Deleted (or tried to) "+str(n)+" edges and "+str((n*(n-1))/2+n)+" ("+str(((n*(n-1))/2+n)*2)+" both ways) edges!")