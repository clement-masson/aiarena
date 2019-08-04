from libcpp cimport bool
from libcpp.string cimport string
from libcpp.vector cimport vector
from libcpp.pair cimport pair

from .move cimport CMove, Move
from .cell cimport Cell

cdef extern from "CBoardState.h" namespace "Chess":
    cdef const int NROWS
    cdef const int NCOLUMNS
    cdef const int NCELLS


    cdef cppclass CBoardState:
        CBoardState() except +
        CBoardState(CBoardState&) except +
        vector[Cell] cells

        void reverse()
        # string toString()

        bool isValidIndex(int)
        bool isValidRC(int, int)
        Cell getCell(int) except +
        Cell getCell(int, int) except +
        int RCtoIndex(int, int)
        pair[int,int] indexToRC(int)

        vector[CMove*] findPossibleMoves(bool)
        void doMove(CMove& m) except +


cdef class BoardState:
    cdef CBoardState* cBoardState      # hold a C++ instance which we're wrapping
    cdef debug
