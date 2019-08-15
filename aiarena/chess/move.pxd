from libcpp cimport bool
from libcpp.string cimport string

cdef extern from "CMove.h" namespace "Chess":
    cdef cppclass CMove:
        int from_index, to_index
        bool isCapture
        char promotionType
        bool enpassant
        int castle

        CMove(int, int, bool, char, bool, int) except +
        string toPDN()
        void reverse()


cdef class Move:
    cdef CMove* cMove

    @staticmethod
    cdef wrap(CMove* m)
