from libcpp cimport bool
from libcpp.string cimport string

cdef extern from "CCell.h" namespace "Chess":
    cdef cppclass Cell:
        char pieceType
        bool isWhite

        CMove(char, bool) except +

cdef class PyCell:
    cdef Cell* _cell

    @staticmethod
    cdef wrap(Cell* c)
