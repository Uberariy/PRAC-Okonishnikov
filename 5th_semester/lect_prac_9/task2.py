import sys
l = (sys.stdin.read()).split("\n")
if len(l[-1]) == 0:
    l = l[0:-1]
try:
    for i in l[:-1:]:
        o = i.split(" ")
        if len(o)==3:
            o[0] += "("+",".join(o[1])+")"
            o[1] = o[2]
        exec("class "+o[0]+":\n\t"+"".join(["\n\tdef "+j+'():\n\t\tpass ' for j in o[1]])+"\n\tpass")

    o = l[-1].split(" ")
    exec("class Pepelaz("+",".join(o[0])+"):\n\t"+"".join(["\n\tdef "+j+'():\n\t\tpass ' for j in o[1]])+"\n\tpass\np = Pepelaz()\n"+"".join(["p."+j+'; ' for j in o[2]]))
except Exception:
    print("Incorrect")
else:
    print("Correct")