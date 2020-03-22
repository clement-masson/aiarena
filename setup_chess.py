from distutils.extension import Extension

C_sources = ['aiarena/chess/src/CGameState.cpp',
             'aiarena/chess/src/CCell.cpp',
             'aiarena/chess/src/CMove.cpp']

def get_extensions(use_cython):
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
    return extensions
