import os
from setuptools import setup, find_packages
import setup_chess
import setup_checkers
import setup_connect4
import setup_abalone

print("Gathering extensions")
use_cython = int(os.getenv('USE_CYTHON', '0'))
extensions = []
for module in [setup_chess, setup_checkers, setup_connect4, setup_abalone]:
    extensions += module.get_extensions(use_cython)

if use_cython:
    print("Cythonizing extensions")
    from Cython.Build import cythonize
    extensions = cythonize(extensions, language_level="3")

print('Installing package')
setup(name='aiarena',
      version='1.0',
      description='Game simulator for 2 players games',
      author='Clement Masson',
      author_email='masson.cle@gmail.com',
      url='',
      license='MIT',
      packages=find_packages(),
      install_requires=[
          'colorama',
      ],
      ext_modules=extensions
      )
