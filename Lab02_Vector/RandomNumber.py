import random

for k in range(10):
    with open("input_"+str(k) + ".txt","w") as file:
        for i in range(random.randint(1,100)):
            file.write(str(random.randint(1,5000))+" ")