from os import system
import numpy as np
import random
import sys

if __name__ == "__main__":
    if len (sys.argv) > 1:
        n = int(sys.argv[1])
    else:
        print("=(1/3)> Could you please type power of the fullmesh net, that would be created?")
        print("-> ", end = '')
        n = int(input())
print("=(1/3)> Don't forget to delete the net!")

#   1: Create fullmesh graph based net
# Create edges (n-1)*2+n*2:
edges = []
for i in range(1, n+1, 1):
    for j in range(i+1, n+1, 1):
        edges += ["sw"+str(i)+"-sw"+str(j), "sw"+str(j)+"-sw"+str(i)]
        system("ip link add sw"+str(i)+"-sw"+str(j)+" type veth peer name sw"+str(j)+"-sw"+str(i))
    edges += ["sw"+str(i)+"-ns"+str(i), "ns"+str(i)+"-sw"+str(i)]
    system("ip link add ns"+str(i)+"-sw"+str(i)+" type veth peer name sw"+str(i)+"-ns"+str(i))
#print(edges)

# Create bridges=switches:
swchs = []
for i in range(1, n+1, 1):
    swchs += ["sw"+str(i)]

for swch in swchs:
    system("ovs-vsctl add-br "+swch)
    system("ovs-vsctl set-fail-mode "+swch+" secure")
    system("ovs-vsctl set bridge "+swch+" other-config:disable-in-band=true")
    system("ovs-vsctl set bridge "+swch+" protocols=OpenFlow13")

# Connect bridges and edges:
for i in range(1, n+1, 1):
    system("ovs-vsctl add-port sw"+str(i)+" sw"+str(i)+"-ns"+str(i)+" -- set interface sw"+str(i)+"-ns"+str(i)+" ofport_request="+str(i))
    for j in range(i+1, n+1, 1):
        system("ovs-vsctl add-port sw"+str(i)+" sw"+str(i)+"-sw"+str(j)+" -- set interface sw"+str(i)+"-sw"+str(j)+" ofport_request="+str(j))
    for j in range(1, i, 1):
        system("ovs-vsctl add-port sw"+str(i)+" sw"+str(i)+"-sw"+str(j)+" -- set interface sw"+str(i)+"-sw"+str(j)+" ofport_request="+str(j))

# Activate edges:
for edge in edges:
    system("ifconfig "+edge+" up")

# Create a namespace=client for each switch, give ip:
for i in range(1, n+1, 1):
    system("ip netns add ns"+str(i))
    system("ip link set ns"+str(i)+"-sw"+str(i)+" netns ns"+str(i))

for i in range(1, n+1, 1):  
    system("ip netns exec ns"+str(i)+" ifconfig ns"+str(i)+"-sw"+str(i)+" 10.0.0."+str(i))

# Set apr rules
for i in range(1, n+1, 1):
    system("ovs-ofctl add-flow sw"+str(i)+" -O openflow13 arp,arp_tpa=10.0.0."+str(i)+",action=output:"+str(i))
    for j in range(i+1, n+1, 1):
        system("ovs-ofctl add-flow sw"+str(i)+" -O openflow13 arp,arp_tpa=10.0.0."+str(j)+",action=output:"+str(j))
    for j in range(1, i, 1):
        system("ovs-ofctl add-flow sw"+str(i)+" -O openflow13 arp,arp_tpa=10.0.0."+str(j)+",action=output:"+str(j))

# Set ip rules:
for i in range(1, n+1, 1):
    system("ovs-ofctl add-flow sw"+str(i)+" -O openflow13 ip,nw_dst=10.0.0."+str(i)+",action=output:"+str(i))
    for j in range(i+1, n+1, 1):
        system("ovs-ofctl add-flow sw"+str(i)+" -O openflow13 ip,nw_dst=10.0.0."+str(j)+",action=output:"+str(j))
    for j in range(1, i, 1):
        system("ovs-ofctl add-flow sw"+str(i)+" -O openflow13 ip,nw_dst=10.0.0."+str(j)+",action=output:"+str(j))

print("\n=(1/3)> Net creation is finished!")
print("=(2/3)=>  Insert 2 positive integers (matrix by default has numbers between 1 and 100):")
print("=(2/3)>     Value that limits length of a path between i&j:")
print("=(2/3)=>       [IT MAY BE ANY INTEGER] If exceeded, direct way would be chosen:")
print("-> ", end = '')
maxval = int(input())
print("\n=(2/3)>     Number of hopes between i&j:")
print("=(2/3)=>       [IT MUST BE LESS THAN ",n,"] If exceeded, direct way would be chosen:")
print("-> ", end = '')
maxhopes = int(input())
Matrix = np.zeros((n, n), dtype=int)
for i in range(0, n, 1):
    for j in range(0, i, 1):
        Matrix[i,j] = random.randint(1,100)
    for j in range(i+1, n, 1):
        Matrix[i,j] = random.randint(1,100)

print(Matrix)
#This block is described further
PathMatrix = np.zeros((n, n), dtype=float)
def PathTest(dontgo, start, end, depth, burden=0):  # Start is the initial point on every level of depthness
    # Depth of recursion
    if depth == maxhopes:
        a = [burden + Matrix[start, end]]
        a += [dontgo[0]*100+dontgo[2]]
        for j in range(2, len(dontgo)-1, 1):
            a += [dontgo[j]*100+dontgo[j+1]]
        a += [dontgo[len(dontgo)-1]*100+dontgo[1]]
        return a

    # Finding the best route:
    minpath = 65000;    # Big enough
    for i in range(0, n, 1):
        if i in dontgo:
            continue  
        else:
            Potato = PathTest(dontgo+[i], i, end, depth+1, burden+Matrix[start,i])
            if Potato[0] < minpath:
                minpath = Potato[0]
                Potatomin = Potato
    return Potatomin

for i in range(0, n, 1):
    for j in range(0, n, 1):  
        if j == i:
            continue
        Pathh = PathTest([i, j], i, j, 1, 0)
        if Pathh[0] > maxval:    # First value of Pathh is the length
            #print("->",Pathh)
            PathMatrix[i,j] += 1
        else:
            #print("-<",Pathh)
            eddgesnum = len(list(Pathh))-1
            for eddge in Pathh:
                if eddge == Pathh[0]:
                    continue
                PathMatrix[eddge//100,eddge%100] += float("{0:.2f}".format(1/eddgesnum))

print("""
=(2/3)>  Next matrix shows, how each edge is valuable for the net: 
=(2/3)=>   Each edge gets +1/hopes for each successful route;
=(2/3)>      Route between i&j is successful if it is the shortest. \n""")
print(PathMatrix)
PathMatrix += PathMatrix.transpose()
print("""
=(2/3)>  Next matrix contains info, about which edges are never used: 
=(2/3)=>   If an edge from i to j has a 0 value, it will be inactivated; \n""")
print(PathMatrix)

edges = []
for i in range(0, n, 1):
    for j in range(i+1, n, 1):
        if PathMatrix[i,j] == 0:
            edges += ["sw"+str(i+1)+"-sw"+str(j+1)]
for edge in edges:
	system("ip link del "+edge)

print("\n=(3/3)>  Edges ",edges," were deleted!\n")

# Net diagnostics:
#print("\n-(1/3)>-[ifconfig -a]---------------[NET DIAGNOSTICS]---------------[ifconfig -a]------")
#system("ifconfig -a")
#print("\n-(1/3)>-[ovs-vsctl show]------------[NET DIAGNOSTICS]------------[ovs-vsctl show]------")
#system("ovs-vsctl show")