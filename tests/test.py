import stack
import sys
import weakref
import unittest
import timeit

class TestObject():
    pass

class TestStack(unittest.TestCase):

    def test_pushpop(self):
        s = stack.Stack()
        for i in range(100, 1000, 3):
            s.push(i)
        for i in range(1000, 100, 3):
            self.assertEqual(i, s.pop())
    
    def test_pop(self):
        s = stack.Stack()
        s.push(123)
        s.pop()
        with self.assertRaises(IndexError):
            s.pop()

    def test_peek(self):
        s = stack.Stack()
        for i in range(100, 1000, 3):
            s.push(i)
            self.assertEqual(i, s.peek())

    def test_clear(self):
        s = stack.Stack()
        for i in range(100):
            s.push(i)
        s.clear()
        self.assertEqual(0, len(s))
        with self.assertRaises(IndexError):
            s.pop()
    
    def test_len(self):
        s = stack.Stack()
        for i in range(100):
            s.push(i)
        self.assertEqual(100, len(s))
        s.pop()
        self.assertEqual(99, len(s))
        s.peek()
        self.assertEqual(99, len(s))
        s.push("asdas")
        self.assertEqual(100, len(s))
        s.clear()
        self.assertEqual(0, len(s))
    
    def test_refs(self):
        obj = TestObject()
        ref = weakref.ref(obj)
        s = stack.Stack()
        s.push(obj)
        self.assertEqual(3, sys.getrefcount(obj))
        s.pop()
        self.assertEqual(2, sys.getrefcount(obj))
        s.push(obj)
        s.clear()
        self.assertEqual(2, sys.getrefcount(obj))
        s.push(obj)
        del obj
        self.assertEqual(2, sys.getrefcount(ref()))
        s.pop()
        self.assertIs(None, ref())
    
    def count_iterations(self, iterations: int):
        print(iterations, "z nen")
        t1 = """
from stack import Stack
s = Stack()
for i in range(1, iterations):
    s.push(i)
    if (i % 150) == 0:
        for j in range(100):
            s.pop()
        """
        t2 = """
from collections import deque
s = deque()
for i in range(1, iterations):
    s.append(i)
    if (i % 150) == 0:
        for j in range(100):
            s.pop()
        """
        time_stack = timeit.timeit(t1, number=10, globals={'iterations': iterations}) / 10
        time_deque = timeit.timeit(t2, number=10, globals={'iterations': iterations}) / 10
        print(f"На {iterations} итерациях - Время стека: {time_stack}с, время deque: {time_deque}с")
        if time_stack > time_deque:
            print(f"    Мы отстаём на {time_stack / time_deque * 100 - 100:.2f}%")
        else:
            print(f"    Мы впереди на {time_deque / time_stack * 100 - 100:.2f}%")

    def test_speed(self):
        a = 1
        for i in range(8):
            self.count_iterations(a)
            a *= 10

if __name__ == '__main__':
    unittest.main()
