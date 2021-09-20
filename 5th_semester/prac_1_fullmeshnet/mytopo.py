from os import system
import sys
#todo del previous nets

if __name__ == "__main__":
    if len (sys.argv) > 1:
        n = int(sys.argv[1])
    else:
        print("-> Could you please type power of the fullmesh net, that would be created?")
        print("==> ", end = '')
        n = int(input())
print("-> Don't forget to delete the net!")

#   1: Create fullmesh graph based net
#Create edges (n-1)*2+n*2:
edges = []
for i in range(1, n+1, 1):
    for j in range(i+1, n+1, 1):
        edges += ["sw"+str(i)+"-sw"+str(j), "sw"+str(j)+"-sw"+str(i)]
        system("ip link add sw"+str(i)+"-sw"+str(j)+" type veth peer name sw"+str(j)+"-sw"+str(i))
#    for j in range(1, i, 1):
#        edges += ["sw"+str(i)+"-sw"+str(j), "sw"+str(j)+"-sw"+str(i)]
    edges += ["sw"+str(i)+"-ns"+str(i), "ns"+str(i)+"-sw"+str(i)]
    system("ip link add ns"+str(i)+"-sw"+str(i)+" type veth peer name sw"+str(i)+"-ns"+str(i))
#print(edges)

#Create bridges=switches:
swchs = []
for i in range(1, n+1, 1):
    swchs += ["sw"+str(i)]

for swch in swchs:
    system("ovs-vsctl add-br "+swch)
    system("ovs-vsctl set-fail-mode "+swch+" secure")
    #system("ovs-vsctl set bridge "+swch+" other-config:disable-in-band=true")
    system("ovs-vsctl set bridge "+swch+" protocols=OpenFlow13")

#Connect bridges and edges:
for i in range(1, n+1, 1):
    system("ovs-vsctl add-port sw"+str(i)+" sw"+str(i)+"-ns"+str(i)+" -- set interface sw"+str(i)+"-ns"+str(i)+" ofport_request="+str(i))
    for j in range(i+1, n+1, 1):
        system("ovs-vsctl add-port sw"+str(i)+" sw"+str(i)+"-sw"+str(j)+" -- set interface sw"+str(i)+"-sw"+str(j)+" ofport_request="+str(j))
    for j in range(1, i, 1):
        system("ovs-vsctl add-port sw"+str(i)+" sw"+str(i)+"-sw"+str(j)+" -- set interface sw"+str(i)+"-sw"+str(j)+" ofport_request="+str(j))

#Activate edges:
for edge in edges:
    system("ifconfig "+edge+" up")

#Create a namespace=client for each switch, give ip:
for i in range(1, n+1, 1):
    system("ip netns add ns"+str(i))
    system("ip link set ns"+str(i)+"-sw"+str(i)+" netns ns"+str(i))

for i in range(1, n+1, 1):  
    system("ip netns exec ns"+str(i)+" ifconfig ns"+str(i)+"-sw"+str(i)+" 10.0.0."+str(i))

#Set ip rules:
for i in range(1, n+1, 1):
    system("ovs-ofctl add-flow sw"+str(i)+" -O openflow13 ip,nw_dst=10.0.0."+str(i)+",action=output:"+str(i))
    for j in range(i+1, n+1, 1):
        system("ovs-ofctl add-flow sw"+str(i)+" -O openflow13 ip,nw_src=10.0.0."+str(j)+",action=output:"+str(j))
    for j in range(1, i, 1):
        system("ovs-ofctl add-flow sw"+str(i)+" -O openflow13 ip,nw_dst=10.0.0."+str(j)+",action=output:"+str(j))

#NET DIAGNOSTICS
print("\n-----[ifconfig -a]---------------[NET DIAGNOSTICS]---------------[ifconfig -a]-----")
system("ifconfig -a")
print("\n-----[ovs-vsctl show]------------[NET DIAGNOSTICS]------------[ovs-vsctl show]-----")
system("ovs-vsctl show")
#print("----------------------------------------------------------------")
#system("ovs-ofctl show sw1 -O openflow13")
#print("----------------------------------------------------------------")
#system("ovs-ofctl show sw2 -O openflow13")
#print("----------------------------------------------------------------")
#system("ovs-ofctl show sw3 -O openflow13")

