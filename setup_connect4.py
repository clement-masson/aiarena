from distutils.extension import Extension

C_sources = ['aiarena/connect4/src/CGameState.cpp',
             'aiarena/connect4/src/CCell.cpp',
             'aiarena/connect4/src/CMove.cpp']

def get_extensions(use_cython):
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
    return extensions
