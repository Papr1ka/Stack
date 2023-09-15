

from setuptools import setup, find_packages, Extension
from stack import __version__


def readme():
  with open('README.md', 'r') as f:
    return f.read()

setup(
    name="stack",
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
    package_data={"stack": ["*.pyi"]},
    ext_modules=[
        Extension(
            name="_stack",
            sources=["stack/_stackmodule.c"],
        )
    ],
)
