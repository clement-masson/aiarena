import setup_chess
import setup_checkers
import setup_connect4
import setup_abalone
from Cython.Build import cythonize

print("Gathering extensions")
extensions = []
for module in [setup_chess, setup_checkers, setup_connect4, setup_abalone]:
    extensions += module.get_extensions(use_cython=True)

print("Cythonizing extensions")
extensions = cythonize(extensions, language_level="3")
print("Done")
