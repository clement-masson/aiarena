import os
from distutils.extension import Extension

USE_CYTHON = bool(os.getenv('USE_CYTHON'))
ext = '.pyx' if USE_CYTHON else '.cpp'

C_sources = ['aiarena/checkers/src/CBoardState.cpp',
             'aiarena/checkers/src/CCell.cpp',
             'aiarena/checkers/src/CMove.cpp']

extensions = [
    Extension(
        name="aiarena.checkers.boardState",
        language='c++',
        sources=["aiarena/checkers/boardState" + ext] + C_sources,
        include_dirs=["aiarena/checkers/src"],
        undef_macros=["NDEBUG"]
    ),
    Extension(
        name="aiarena.checkers.move",
        language='c++',
        sources=["aiarena/checkers/move" + ext] + C_sources,
        include_dirs=["aiarena/checkers/src"]
    )
]

if USE_CYTHON:
    print("[SETUP_CHECKERS] cythonizing extensions")
    from Cython.Build import cythonize
    extensions = cythonize(extensions)
