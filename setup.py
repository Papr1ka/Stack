from distutils.core import setup, Extension

module = Extension('stack', ['stackmodule.c'])

setup(name='StackPackage',
    version='0.0.2',
    ext_modules=[module])