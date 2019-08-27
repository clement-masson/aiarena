import os
from distutils.extension import Extension

USE_CYTHON = bool(os.getenv('USE_CYTHON'))
ext = '.pyx' if USE_CYTHON else '.cpp'

C_sources = ['aiarena/abalone/src/CGameState.cpp',
             'aiarena/abalone/src/CCell.cpp',
             'aiarena/abalone/src/CMove.cpp']

extensions = [
    Extension(
        name="aiarena.abalone.cell",
        language='c++',
        sources=["aiarena/abalone/cell" + ext] + C_sources,
        include_dirs=["aiarena/abalone/src"]
    ),
    Extension(
        name="aiarena.abalone.gameState",
        language='c++',
        sources=["aiarena/abalone/gameState" + ext] + C_sources,
        include_dirs=["aiarena/abalone/src"],
        undef_macros=["NDEBUG"]
    ),
    Extension(
        name="aiarena.abalone.move",
        language='c++',
        sources=["aiarena/abalone/move" + ext] + C_sources,
        include_dirs=["aiarena/abalone/src"]
    )
]

if USE_CYTHON:
    print("[SETUP_ABALONE] cythonizing extensions")
    from Cython.Build import cythonize
    extensions = cythonize(extensions)
