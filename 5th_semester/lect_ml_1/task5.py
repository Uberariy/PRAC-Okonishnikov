def primes():    
    def prime(a):
        b = 2
        while a%b != 0:
            b += 1
        return (a == b)
    i = 2
    while(1):
        if prime(i):
            yield i
        i += 1

if __name__ == "__main__":
    for i in primes():
        print(i)
        if i > 10:
            break
