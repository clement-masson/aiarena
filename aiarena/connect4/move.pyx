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

    def toPDN(self):
        return self.cMove.toPDN().decode('UTF-8')

    def __repr__(self):
      return self.toPDN()
