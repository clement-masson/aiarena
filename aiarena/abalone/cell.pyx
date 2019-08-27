from .cell cimport *

NONE = chr(cNONE)
WHITE = chr(cWHITE)
BLACK = chr(cBLACK)

cdef class Cell:

    @staticmethod
    cdef wrap(CCell c):
        result = Cell()
        result.cCell = c
        return result

    @property
    def color(self):
        return chr(self.cCell.color)

    def __repr__(self):
        return self.color
