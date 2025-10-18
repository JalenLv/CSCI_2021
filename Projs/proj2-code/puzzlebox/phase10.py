with open("input.txt", "r") as f:
    userID = f.readlines()[0].strip()

a = ""
for i in range(len(userID)):
    tmp = bin(ord(userID[i]))[2:].zfill(7)
    print(f"{userID[i]}: {tmp}")
    a = tmp + a

print(a)
a = int(a, 2)
print(a)

