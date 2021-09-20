import networkx as nx
import numpy as np
import matplotlib.pyplot as plt
import random

#   2: Some graph theory
n = 6 # less than 100
maxhopes = 5 # less than n
maxval = 25 # any
Matrix = np.zeros((n, n), dtype=int)
for i in range(0, n, 1):
    for j in range(0, i, 1):
        Matrix[i,j] = random.randint(1,10)
    for j in range(i+1, n, 1):
        Matrix[i,j] = random.randint(1,10)

print(Matrix)
# Next matrix shows, how each edge is valuable for the net: 
#   Each edge gets +1/hopes for each successful route;
#      Route between i&j is successful if it is the shortest.
PathMatrix = np.zeros((n, n), dtype=float)
def PathTest(dontgo, start, end, depth, burden=0):  # Start is the initial point on every level of depthness
    if depth == maxhopes:
        #print(dontgo)
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
                tmpi = i
    return Potato# + [start*100+tmpi]

#print("Test",PathTest([1, 2], 1, 2, 1, 0))

for i in range(0, n, 1):
    for j in range(0, n, 1):  
        if j == i:
            continue
        Pathh = PathTest([i, j], i, j, 1, 0)
        if Pathh[0] > maxval:    # First value of Pathh is the length
            print("->",Pathh)
            PathMatrix[i,j] += 1
        else:
            print("-<",Pathh)
            eddgesnum = len(list(Pathh))-1
            for eddge in Pathh:
                if eddge == Pathh[0]:
                    continue
                #print(float("{0:.2f}".format(1/eddgesnum)))
                PathMatrix[eddge//100,eddge%100] += float("{0:.2f}".format(1/eddgesnum))

print(PathMatrix)

G = nx.from_numpy_matrix(np.matrix(Matrix), create_using=nx.DiGraph)
layout = nx.spring_layout(G)
labels = nx.get_edge_attributes(G, "weight")
nx.draw_networkx_edge_labels(G, pos=layout, edge_labels=labels)
nx.draw(G, layout)
nx.draw_networkx_edge_labels(G, pos=layout)
plt.show()