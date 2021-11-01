from libcpp cimport bool
from libcpp.string cimport string

cdef extern from "CCell.h" namespace "Abalone":
    cdef cppclass CCell:
        char color

        CCell() except +
        CCell(char) except +
        string toString()

cdef extern from "CCell.h" :
    cdef const char cNONE "Abalone::CCell::NONE"
    cdef const char cWHITE "Abalone::CCell::WHITE"
    cdef const char cBLACK "Abalone::CCell::BLACK"

cdef class Cell:
    cdef CCell cCell

    @staticmethod
    cdef wrap(CCell c)
