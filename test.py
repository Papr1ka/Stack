import unittest
import stack
import sys
import weakref

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
        print(obj)
        # self.assertEqual(2, sys.getrefcount(obj))
        # s.push(obj)
        # s.clear()
        # self.assertEqual(2, sys.getrefcount(obj))
        # s.push(obj)
        # del obj
        # self.assertEqual(2, sys.getrefcount(ref()))
        # s.pop()
        # self.assertIs(None, ref())


if __name__ == '__main__':
    unittest.main()
