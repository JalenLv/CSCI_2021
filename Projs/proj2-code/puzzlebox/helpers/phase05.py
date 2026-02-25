t = 25
for n in range(100000):
    s = 0
    while n > 1:
        if n & 1 != 0:
            n = (n << 2) - n + 1
        else:
            n = n >> 1
        if s == t:
            print(n)
            exit()
        s += 1

