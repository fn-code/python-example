listData = ["hello", "ludin"]
print("List of data : ", listData)

for x in listData:
    print("value is : ", x)

for n in range(2, 10):
    for x in range(2, n):
        if n % x == 0:
            print(n, 'equals', x, '*', n//x)
            break
    else:
        # loop fell through without finding a factor
        print(n, 'is a prime number')
