from distutils.extension import Extension

C_sources = ['aiarena/checkers/src/CGameState.cpp',
             'aiarena/checkers/src/CCell.cpp',
             'aiarena/checkers/src/CMove.cpp']

def get_extensions(use_cython):
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
    return extensions
