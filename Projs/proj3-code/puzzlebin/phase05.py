def isOdd(val):
    return (val & 1) == 1
def fail(inp):
    pass
def success(inp):
    print(f"0x{inp:08x} SUCCESS")

def test(inp):
    eax = 277
    ecx = 0
    esi = 0
    while esi <= 2:
        edx = 1 << ecx
        edx = edx & inp
        if isOdd(eax):
            if edx != 0:
                esi += 1
                eax = eax * 3 + 1
                ecx += 1
            else:
                fail(inp)
                return
        else:
            if edx == 0:
                eax = eax >> 1
                ecx += 1
            else:
                fail(inp)
                return
    success(inp)

def main():
    for i in range(0, 0x10000):
        test(i)

if __name__ == "__main__":
    main()

