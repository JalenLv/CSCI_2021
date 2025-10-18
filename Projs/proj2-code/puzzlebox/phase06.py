hash = 84035164
SIZEOF_INT = 4

expect = "There are 10 kinds of people in the world: those who understand binary and those who don't"

slen = len(expect)
ilen = slen // SIZEOF_INT - 1
munge1 = (hash % 211) % ilen
munge2 = (hash % 311) % ilen

a = munge1
# translate c to python
# b = ((int *) expect)[munge1]
b = ord(expect[munge1 * SIZEOF_INT]) | (ord(expect[munge1 * SIZEOF_INT + 1]) << 8) | (ord(expect[munge1 * SIZEOF_INT + 2]) << 16) | (ord(expect[munge1 * SIZEOF_INT + 3]) << 24)
c = munge2
# d = ((int *) expect)[munge2]
d = ord(expect[munge2 * SIZEOF_INT]) | (ord(expect[munge2 * SIZEOF_INT + 1]) << 8) | (ord(expect[munge2 * SIZEOF_INT + 2]) << 16) | (ord(expect[munge2 * SIZEOF_INT + 3]) << 24)

print(a, b, c, d)

