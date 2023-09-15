from setuptools import setup, find_packages, Extension
from sstack import __version__

PACKAGE_NAME = "sstack"

def readme():
  with open('README.md', 'r') as f:
    return f.read()

setup(
    name=PACKAGE_NAME,
    version=__version__,
    author="Papr1ka",
    author_email="kirillpavlov4214@gmail.com",
    description="The simple module with stack container",
    long_description=readme(),
    long_description_content_type="text/markdown",
    url="",
    packages=find_packages(),
    classifiers=[
    'Programming Language :: Python :: 3.9',
    'License :: OSI Approved :: MIT License',
    'Operating System :: OS Independent'
    ],
    keywords='simple stack python',
    package_data={PACKAGE_NAME: ["*.pyi"]},
    ext_modules=[
        Extension(
            name="_stack",
            sources=[f"{PACKAGE_NAME}/_stackmodule.c"],
        )
    ],
)
