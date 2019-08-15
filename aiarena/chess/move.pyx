from cython.operator cimport dereference as deref, preincrement as inc

from .move cimport *
import re

pdn_pattern = re.compile('(?P<from>\d+)(?P<type>[x-])(?P<to>\d+)(?P<promotion>\w)?')

cdef class Move:

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
