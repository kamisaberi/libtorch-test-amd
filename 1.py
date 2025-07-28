import time
s = 0
st = time.time()
for i in range(1000000000):
	s+=i
	if i%10000000 == 0:
		print("inside:",s)

print(s)
et = time.time()
print(st-et)

