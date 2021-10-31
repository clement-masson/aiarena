from libcpp cimport bool
from libcpp.string cimport string

cdef extern from "CCell.h" namespace "Chess":
    cdef cppclass CCell:
        char pieceType
        bool isWhite

        CCell() except +
        CCell(char, bool) except +
        string toString()

cdef extern from "CCell.h" namespace "Chess::PieceType":
    cdef const char none
    cdef const char pawn
    cdef const char rook
    cdef const char knight
    cdef const char bishop
    cdef const char queen
    cdef const char king

cdef class Cell:
    cdef CCell cCell

    @staticmethod
    cdef wrap(CCell c)
