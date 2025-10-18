hash = 84035164
a = 1
b = 2

a += hash % 26
v = a + b
v *= a
v //= b
v += 14

c = v
print(c)
