def fail():
    return 1

for origin_n in range(10000):
    t = 92
    m = 92
    s = 0
    n = origin_n
    while True:
        # LTOP
        if not (n > 1):
            # LEND
            break
        #LOC1
        if n & 1 != 0:
            # LOM1
            n = (n << 2) - n + 1
        # LEM1
        n = n >> 1
        # LCHK
        if s == t and m == t:
            # LAFT
            print(origin_n)
            exit(0)
        s += 1
        #LTOP
