from distutils.core import setup, Extension

module = Extension('custom3', ['custom.c'])

setup(name='TestPackage',
    version='0.0.1',
    ext_modules=[module])
