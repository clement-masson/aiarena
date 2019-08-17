from cython.operator cimport dereference as deref, preincrement as inc
from .cell cimport *


cdef class Cell:

    def __cinit__(self):
        pass

    def __dealloc__(self):
        del self.cCell

    @staticmethod
    cdef wrap(CCell* c):
        result = Cell()
        result.cCell = c
        return result

    def isWhite(self):
        return self.cCell.isWhite

    def type(self):
        return chr(self.cCell.pieceType)
