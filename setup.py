from setuptools import setup, find_packages
import distutils.sysconfig
from setup_checkers import extensions as checkers_extensions
from setup_chess import extensions as chess_extensions
from setup_connect4 import extensions as connect4_extensions
from setup_abalone import extensions as abalone_extensions

print("Configuring distutils")
# Remove the "-Wstrict-prototypes" compiler option, which isn't valid for C++.
# merci pierre.aubert@lapp.in2p3.fr
cfg_vars = distutils.sysconfig.get_config_vars()
for key, value in cfg_vars.items():
    if isinstance(value, str):
        value = value.replace("-Wstrict-prototypes", "")
        value = value.replace("-DNDEBUG", "")
        cfg_vars[key] = value

extensions = (
      checkers_extensions 
    + chess_extensions
    + connect4_extensions
    + abalone_extensions
)

print('Installing package')
setup(name='aiarena',
      version='1.0',
      description='Game simulator for 2 players games',
      author='Clement Masson',
      author_email='masson.cle@gmail.com',
      url='',
      license='MIT',
      packages=find_packages(),
      install_requires=['colorama'],
      ext_modules=extensions
      )
