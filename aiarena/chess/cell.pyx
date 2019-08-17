from .cell cimport *

NONE= chr(none)
PAWN = chr(pawn)
ROOK = chr(rook)
KNIGHT = chr(knight)
BISHOP = chr(bishop)
QUEEN = chr(queen)
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
        return ('w' if self.isWhite else 'b') + '_' + self.type
