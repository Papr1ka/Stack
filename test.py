import weakref

class Test():
    name = "Боря"

a = Test()

b = weakref.ref(a)
c = a
print(b, b())

del a
print(b, b())
print(c)
