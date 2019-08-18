import os
from distutils.extension import Extension

USE_CYTHON = bool(os.getenv('USE_CYTHON'))
ext = '.pyx' if USE_CYTHON else '.cpp'

C_sources = ['aiarena/connect4/src/CGameState.cpp',
             'aiarena/connect4/src/CCell.cpp',
             'aiarena/connect4/src/CMove.cpp']

extensions = [
    Extension(
        name="aiarena.connect4.cell",
        language='c++',
        sources=["aiarena/connect4/cell" + ext] + C_sources,
        include_dirs=["aiarena/connect4/src"]
    ),
    Extension(
        name="aiarena.connect4.gameState",
        language='c++',
        sources=["aiarena/connect4/gameState" + ext] + C_sources,
        include_dirs=["aiarena/connect4/src"],
        undef_macros=["NDEBUG"]
    ),
    Extension(
        name="aiarena.connect4.move",
        language='c++',
        sources=["aiarena/connect4/move" + ext] + C_sources,
        include_dirs=["aiarena/connect4/src"]
    )
]

if USE_CYTHON:
    print("[SETUP_CONNECT4] cythonizing extensions")
    from Cython.Build import cythonize
    extensions = cythonize(extensions)
