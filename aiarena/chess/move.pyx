from cython.operator cimport dereference as deref, preincrement as inc

from .move cimport *
import re

pdn_pattern = re.compile('(?P<from>\d+)(?P<type>[x-])(?P<to>\d+)(?P<promotion>\w)?')

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

    def reverse(self):
        self.cMove.reverse()

    # @staticmethod
    # def fromPDN(s):
    #     if s == 'o-o':
    #       return Move.wrap(new CMove(0,0,False,' ', False, -1))
    #     if s == 'O-O':
    #       return Move.wrap(new CMove(0,0,False,' ', False, 1))
    #
    #     assert(s.rfind('x')>-1 or s.rfind('-')>-1)
    #     is_capture = s.rfind('x') >= 0
    #     separator = 'x' if is_capture else '-'
    #     from_index, to_index = [int(i) for i in s.split(separator)]
    #     print(f'creating move from {from_index} to {to_index}')
    #     return Move.wrap(new CMove(from_index, to_index, is_capture, ' ', False, 0))
