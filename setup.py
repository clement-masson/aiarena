#!/usr/bin/env python
import os
from distutils.core import setup
from distutils.extension import Extension

USE_CYTHON = bool(os.getenv('USE_CYTHON'))
ext = '.pyx' if USE_CYTHON else '.cpp'

C_sources = ['ChessSimulator/cpp/CBoardState.cpp',
             'ChessSimulator/cpp/CCell.cpp',
             'ChessSimulator/cpp/CMove.cpp']

extensions = [
    Extension(
        name="ChessSimulator.cpp.cell",
        language='c++',
        sources=["ChessSimulator/cpp/cell" + ext] + C_sources,
        include_dirs=["."]
    ),
    Extension(
        name="ChessSimulator.cpp.boardState",
        language='c++',
        sources=["ChessSimulator/cpp/boardState" + ext] + C_sources,
        include_dirs=["."]
    ),
    Extension(
        name="ChessSimulator.cpp.move",
        language='c++',
        sources=["ChessSimulator/cpp/move" + ext] + C_sources,
        include_dirs=["."]
    )
]

if USE_CYTHON:
    print("Running setup using Cython")
    from Cython.Build import cythonize
    extensions = cythonize(extensions)

setup(name='ChessSimulator',
      version='1.0',
      description='Game simulator for chess',
      author='Clement Masson',
      author_email='masson.cle@gmail.com',
      url='',
      license='MIT',
      packages=['ChessSimulator'],
      ext_modules=extensions
      )
