from setuptools import setup, find_packages
from setup_chess import extensions as chess_extensions
from setup_checkers import extensions as checkers_extensions

extensions = chess_extensions + checkers_extensions

setup(name='aiarena',
      version='1.0',
      description='Game simulator for 2 players games',
      author='Clement Masson',
      author_email='masson.cle@gmail.com',
      url='',
      license='MIT',
      packages=find_packages(),
      ext_modules=extensions
      )
