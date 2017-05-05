#random file generator
#!/usr/bin/python
import random

for i in range(0, 25):
    test_file = open("test_%d.txt"%(i,), "wb")
    for j in range(0, 1000000):
        randNum = random.uniform(97, 122)
        randNum = int(randNum)
        test_file.write(str(randNum))
    test_file.close()
