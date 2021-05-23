from distutils.extension import Extension
import os

use_cython = int(os.getenv('USE_CYTHON', '0'))
if use_cython:
    from Cython.Build import cythonize

C_sources = ['aiarena/connect4/src/CGameState.cpp',
             'aiarena/connect4/src/CCell.cpp',
             'aiarena/connect4/src/CMove.cpp']

ext = '.pyx' if use_cython else '.cpp'
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
        include_dirs=["aiarena/connect4/src"]
    ),
    Extension(
        name="aiarena.connect4.move",
        language='c++',
        sources=["aiarena/connect4/move" + ext] + C_sources,
        include_dirs=["aiarena/connect4/src"]
    )
]

if use_cython:
    extensions = cythonize(extensions, language_level="3")
