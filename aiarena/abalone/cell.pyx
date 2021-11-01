from .cell cimport *

NONE = chr(cNONE)
WHITE = chr(cWHITE)
BLACK = chr(cBLACK)

cdef class Cell:

    def __cinit__(self, color=None):
        if color is not None:
            self.cCell = CCell(ord(color))

    @staticmethod
    cdef wrap(CCell c):
        result = Cell()
        result.cCell = c
        return result

    @property
    def color(self):
        return chr(self.cCell.color)

    def __repr__(self):
        return self.toString()

    def toString(self):
        return self.cCell.toString().decode('UTF-8')
