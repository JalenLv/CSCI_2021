id = "lv000025"
ans = 0
for a in reversed(id):
    ans = (ans << 7) + ord(a)
print(ans)
