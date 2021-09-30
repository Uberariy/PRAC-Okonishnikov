def hello(x=""):
    if x == "":
        return "Hello!"
    return "Hello, " + x + "!"

if __name__ == "__main__":
    print(hello())
    print(hello(''))
    print(hello('Masha'))
