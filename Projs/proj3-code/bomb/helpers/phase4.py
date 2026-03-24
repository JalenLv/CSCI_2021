def func4(di, si, dx):
    ax = dx
    ax -= si
    bx = ax
    bx = (bx & 0xFFFFFFFF) >> 31
    bx += ax
    bx = bx >> 1
    bx += si
    if bx > di:
        dx = bx - 1
        bx += func4(di, si, dx)
    elif bx < di:
        si = bx + 1
        bx += func4(di, si, dx)
    ax = bx
    return ax

for i in range(15):
    if func4(i, 0, 14) == 13:
        print(i)
