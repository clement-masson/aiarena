from distutils.extension import Extension
import os

use_cython = int(os.getenv('USE_CYTHON', '0'))
if use_cython:
    from Cython.Build import cythonize

C_sources = ['aiarena/abalone/src/CGameState.cpp',
             'aiarena/abalone/src/CCell.cpp',
             'aiarena/abalone/src/CMove.cpp']

ext = '.pyx' if use_cython else '.cpp'
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
        include_dirs=["aiarena/abalone/src"]
    ),
    Extension(
        name="aiarena.abalone.move",
        language='c++',
        sources=["aiarena/abalone/move" + ext] + C_sources,
        include_dirs=["aiarena/abalone/src"]
    )
]

if use_cython:
    extensions = cythonize(extensions, language_level="3")
