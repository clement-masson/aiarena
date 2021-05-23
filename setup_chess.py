from distutils.extension import Extension
import os

use_cython = int(os.getenv('USE_CYTHON', '0'))
if use_cython:
    from Cython.Build import cythonize

C_sources = ['aiarena/chess/src/CGameState.cpp',
             'aiarena/chess/src/CCell.cpp',
             'aiarena/chess/src/CMove.cpp']

ext = '.pyx' if use_cython else '.cpp'
extensions = [
    Extension(
        name="aiarena.chess.cell",
        language='c++',
        sources=["aiarena/chess/cell" + ext] + C_sources,
        include_dirs=["aiarena/chess/src"]
    ),
    Extension(
        name="aiarena.chess.gameState",
        language='c++',
        sources=["aiarena/chess/gameState" + ext] + C_sources,
        include_dirs=["aiarena/chess/src"]
    ),
    Extension(
        name="aiarena.chess.move",
        language='c++',
        sources=["aiarena/chess/move" + ext] + C_sources,
        include_dirs=["aiarena/chess/src"]
    )
]

if use_cython:
    extensions = cythonize(extensions, language_level="3")
