from libcpp cimport bool
from libcpp.string cimport string

cdef extern from "CCell.h" namespace "Connect4":
    cdef cppclass CCell:
        char color

        CCell() except +
        CCell(char) except +

cdef extern from "CCell.h" :
    cdef const char cNONE "Connect4::CCell::NONE"
    cdef const char cWHITE "Connect4::CCell::WHITE"
    cdef const char cBLACK "Connect4::CCell::BLACK"

cdef class Cell:
    cdef CCell cCell

    @staticmethod
    cdef wrap(CCell c)
