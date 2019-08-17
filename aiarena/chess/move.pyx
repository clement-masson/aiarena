from cython.operator cimport dereference as deref, preincrement as inc
from .move cimport *


cdef class Move:

    def __cinit__(self):
        pass

    def __dealloc__(self):
        del self.cMove

    @staticmethod
    cdef wrap(CMove* m):
        result = Move()
        result.cMove = m
        return result

    def isCapture(self):
        return self.cMove.isCapture

    def toPDN(self):
        return self.cMove.toPDN().decode('UTF-8')
