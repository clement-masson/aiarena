from distutils.extension import Extension
import os

use_cython = int(os.getenv('USE_CYTHON', '0'))
if use_cython:
    from Cython.Build import cythonize

C_sources = ['aiarena/checkers/src/CGameState.cpp',
             'aiarena/checkers/src/CCell.cpp',
             'aiarena/checkers/src/CMove.cpp']

ext = '.pyx' if use_cython else '.cpp'
extensions = [
    Extension(
        name="aiarena.checkers.cell",
        language='c++',
        sources=["aiarena/checkers/cell" + ext] + C_sources,
        include_dirs=["aiarena/checkers/src"]
    ),
    Extension(
        name="aiarena.checkers.gameState",
        language='c++',
        sources=["aiarena/checkers/gameState" + ext] + C_sources,
        include_dirs=["aiarena/checkers/src"]
    ),
    Extension(
        name="aiarena.checkers.move",
        language='c++',
        sources=["aiarena/checkers/move" + ext] + C_sources,
        include_dirs=["aiarena/checkers/src"]
    )
]

if use_cython:
    extensions = cythonize(extensions, language_level="3")
