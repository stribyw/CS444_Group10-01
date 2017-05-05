#random file generator
#!/usr/bin/python
import random

for i in range(0, 25):
    test_file = open("test_%d.txt"%(i,), "wb")
    test_file.close()

for k in range(0, 50):
    randFile = random.uniform(0, 25)
    randFile = int(randFile)
    test_file = open("test_%d.txt"%(randFile,), "wb")
    for j in range(0, 1000000):
        randNum = random.uniform(97, 122)
        randNum = int(randNum)
        test_file.write(str(randNum))
    test_file.close()

test_file = open("test_2.txt", "r")
test_file.read()
test_file.close()
test_file = open("test_14.txt", "r")
test_file.read()
test_file.close()
test_file = open("test_7.txt", "r")
test_file.read()
test_file.close()
test_file = open("test_5.txt", "r")
test_file.read()
test_file.close()
test_file = open("test_25.txt", "r")
test_file.read()
test_file.close()

for k in range(0, 50):
    randFile = random.uniform(0, 25)
    randFile = int(randFile)
    test_file = open("test_%d.txt"%(randFile,), "wb")
    for j in range(0, 1000000):
        randNum = random.uniform(97, 122)
        randNum = int(randNum)
        test_file.write(str(randNum))
    test_file.close()

test_file = open("test_24.txt", "r")
test_file.read()
test_file.close()
test_file = open("test_13.txt", "r")
test_file.read()
test_file.close()
test_file = open("test_17.txt", "r")
test_file.read()
test_file.close()
test_file = open("test_22.txt", "r")
test_file.read()
test_file.close()
test_file = open("test_0.txt", "r")
test_file.read()
test_file.close()
