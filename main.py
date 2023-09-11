import time
import stack
from collections import deque

"""
s = stack.stack()
print(s.pop())
t = 0

for number in range(1):
    
    start = time.time()

    s = stack.stack()
    for i in range(1, 100000000):
        s.push("124asd")
    
    r = time.time() - start
    t += r

print(t)

"""
t1 = 0
t2 = 0

for number in range(1):
    
    start = time.time()

    s = deque()
    for i in range(1, 10000000):
        s.append("124asd")
        if i % 150 == 0:
            for j in range(150):
                s.pop()
    
    r = time.time() - start
    t1 += r

for number in range(1):
    
    start = time.time()

    s = stack.stack()
    for i in range(1, 10000000):
        s.push("124asd")
        if i % 150 == 0:
            for j in range(150):
                s.pop()
    
    r = time.time() - start
    t2 += r


print(t1, t2)
