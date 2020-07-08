def fib(n):
    a,b = 0,1
    while a < n:
        print(a, end=' ')
        a, b = b, a + b
    print()
fib(2000)

def getName(name):
    print("my name is :", name)

getName("Ludin Neno")

# return function
def sum(a,b):
    return a+b, "hello"

sr, str = sum(2,4)
print("result of sr is : ", sr, " AND ", str)

#  dictonery function
def cheeseshop(kind, *arguments, **keywords):
    print("-- Do you have any", kind, "?")
    print("-- I'm sorry, we're all out of", kind)
    for arg in arguments:
        print(arg)
    print("-" * 40)
    for kw in keywords:
        print(kw, ":", keywords[kw])

cheeseshop("Limburger", "It's very runny, sir.",
           "It's really very, VERY runny, sir.",
           shopkeeper="Michael Palin",
           client="John Cleese",
           sketch="Cheese Shop Sketch")

# lamda make anonymous function
def make_incrementor(n):
    return lambda x: x + n

f = make_incrementor(42)
print(f(1), " ", f(2))

# function anotation
def f1(ham: str, eggs: str = 'eggs') -> str:
    print("Annotations:", f1.__annotations__)
    print("Arguments:", ham, eggs)
    return ham + ' and ' + eggs

f1('spam')






