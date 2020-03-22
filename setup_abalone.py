from distutils.extension import Extension

C_sources = ['aiarena/abalone/src/CGameState.cpp',
             'aiarena/abalone/src/CCell.cpp',
             'aiarena/abalone/src/CMove.cpp']

def get_extensions(use_cython):
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
    return extensions
