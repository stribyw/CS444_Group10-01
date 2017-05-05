#random file generator
#!/usr/bin/python
import random
import shutil



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
test_file = open("test_1.txt", "r")
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

#copy file
shutil.copyfile("lorem.txt", "lorem_copy.txt")

f = open("lorem_copy.txt", "rw+")
f.seek(-39000, 2)
f.write("tacostues1")

f.seek(5005, 1)
f.write("tacostues2")

f.seek(10000, 0)
f.write("tacostues3")

f.seek(15000, 0)
f.write("tacostues3")

f.seek(25500, 0)
f.write("tacostues2")

f.seek(48000, 0)
f.write("tacostues3")

f.seek(5100, 0)
f.write("tacostues3")

f.seek(-2700, 2)
f.write("tacostues4")

f.seek(0, 0)
f.write("tacostues5")

f.seek(0, 2)
f.write("tacostues5")

f.close()
