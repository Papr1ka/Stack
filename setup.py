from distutils.core import setup, Extension

module = Extension('stack', ['Stack.c'])

setup(name='StackPackage',
    version='0.0.1',
    ext_modules=[module])