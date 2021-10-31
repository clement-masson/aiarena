from .cell cimport *

NONE= chr(none)
MAN = chr(man)
KING = chr(king)


cdef class Cell:

    def __cinit__(self, piece_type=None, is_white=None):
        if piece_type is not None:
            if is_white is None: raise Exception('You must provide a piece type AND a color')
            self.cCell = CCell(ord(piece_type), is_white)

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
        return self.toString()

    def toString(self):
        return self.cCell.toString().decode('UTF-8')
