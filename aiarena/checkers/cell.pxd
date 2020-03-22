from libcpp cimport bool
from libcpp.string cimport string

cdef extern from "CCell.h" namespace "Checkers":
    cdef cppclass CCell:
        char pieceType
        bool isWhite

        CCell() except +
        CCell(char, bool) except +

cdef extern from "CCell.h" namespace "Checkers::PieceType":
    cdef const char none
    cdef const char man
    cdef const char king

cdef class Cell:
    cdef CCell cCell

    @staticmethod
    cdef wrap(CCell c)
