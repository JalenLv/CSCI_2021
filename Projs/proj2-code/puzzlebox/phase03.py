hash = 84035164

targ = 1 << ((hash % 7) + 24) | 1 << (hash % 17) | 1 << (hash % 19)
print(bin(targ))

locs = []
for i in range(32):
    if targ & (1 << i):
        locs.append(i)
print(locs)

