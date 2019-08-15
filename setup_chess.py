import os
from distutils.extension import Extension

USE_CYTHON = bool(os.getenv('USE_CYTHON'))
ext = '.pyx' if USE_CYTHON else '.cpp'

C_sources = ['aiarena/chess/src/CBoardState.cpp',
             'aiarena/chess/src/CCell.cpp',
             'aiarena/chess/src/CMove.cpp']

extensions = [
    Extension(
        name="aiarena.chess.cell",
        language='c++',
        sources=["aiarena/chess/cell" + ext] + C_sources,
        include_dirs=["aiarena/chess/src"]
    ),
    Extension(
        name="aiarena.chess.boardState",
        language='c++',
        sources=["aiarena/chess/boardState" + ext] + C_sources,
        include_dirs=["aiarena/chess/src"],
        undef_macros=["NDEBUG"]
    ),
    Extension(
        name="aiarena.chess.move",
        language='c++',
        sources=["aiarena/chess/move" + ext] + C_sources,
        include_dirs=["aiarena/chess/src"]
    )
]

if USE_CYTHON:
    print("[SETUP_CHESS] cythonizing extensions")
    from Cython.Build import cythonize
    extensions = cythonize(extensions)
