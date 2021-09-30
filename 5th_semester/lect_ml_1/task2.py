def is_palindrome(x):
    a = x;
    n = 1;
    while a>9:
        a = a//10
        n += 1;
    for i in range(n//2):
        b = x;
        c = x;
        for j in range(i,n-1,1):
            b = b//10;
        print(b);
        b = b%10;
        for j in range(0,i,1):
            c = c//10;
        c = c%10;
        if c != b:
            return "NO";
    return "YES";

if __name__ == "__main__":
   print(is_palindrome(12345678987654321));
   print(is_palindrome(120));
   print(is_palindrome(10));

