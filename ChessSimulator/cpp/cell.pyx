from cython.operator cimport dereference as deref, preincrement as inc
from .cell cimport *


cdef class PyCell:

    def __cinit__(self):
        pass

    def __dealloc__(self):
        del self._cell

    @staticmethod
    cdef wrap(Cell* c):
        result = PyCell()
        result._cell = c
        return result

    def isWhite(self):
        return self._cell.isWhite

    def type(self):
        return self._cell.pieceType
