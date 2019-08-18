from libcpp cimport bool
from libcpp.string cimport string

cdef extern from "CMove.h" namespace "Connect4":
    cdef cppclass CMove:
        int column

        CMove(int) except +
        string toPDN()


cdef class Move:
    cdef CMove* cMove

    @staticmethod
    cdef wrap(CMove* m)
