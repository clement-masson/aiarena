from libcpp cimport bool
from libcpp.string cimport string

cdef extern from "CCell.h" namespace "Chess":
    cdef cppclass CCell:
        char pieceType
        bool isWhite

        CCell() except +
        CCell(char, bool) except +

cdef class Cell:
    cdef CCell* cCell

    @staticmethod
    cdef wrap(CCell* c)
