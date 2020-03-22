from .cell cimport *

NONE= chr(none)
MAN = chr(man)
KING = chr(king)


cdef class Cell:

    @staticmethod
    cdef wrap(CCell c):
        result = Cell()
        result.cCell = c
        return result

    @property
    def isWhite(self):
        return self.cCell.isWhite

    @property
    def type(self):
        return chr(self.cCell.pieceType)

    def __repr__(self):
        if self.type is NONE:
            return '_'
        return ('w' if self.isWhite else 'b') + '_' + self.type
