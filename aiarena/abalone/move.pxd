from libcpp cimport bool
from libcpp.string cimport string

cdef extern from "CMove.h" namespace "Abalone":
    cdef cppclass CMove:
        int linestart, lineend, destination

        CMove(int, int, int) except +
        string toString()


cdef class Move:
    cdef CMove* cMove

    @staticmethod
    cdef wrap(CMove* m)
