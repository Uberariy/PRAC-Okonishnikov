def squares(w,h,*sq):
    for i in range(h):
        strr = w*["."]
        for k in sq:
            if i>=k[1] and i<k[1]+k[2]:
                strr[k[0]:k[0]+k[2]] = k[3]*k[2]
        print("".join(strr))
    return 0

