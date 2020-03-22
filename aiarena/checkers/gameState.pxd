from libcpp cimport bool
from libcpp.string cimport string
from libcpp.vector cimport vector
from libcpp.pair cimport pair

from .move cimport *

cdef extern from "CGameState.h" namespace "Checkers":
    cdef cppclass CGameState:
        CGameState() except +
        CGameState(int, int, bool, bool, bool) except +
        CGameState(CGameState&) except +
        int nRows, nPieces, nCells
        bool isWhiteTurn
        int noCaptureCounter

        void reverse()
        string toString(bool, bool)

        bool isValidIndex(int)
        bool isValidRC(int, int)
        char getCell(int, int) except +
        vector[char] getCells() except +
        int RCtoIndex(int, int)
        pair[int,int] indexToRC(int)

        vector[CMove*] findPossibleMoves() except +
        void doMove(CMove& m) except +


cdef class GameState:
    cdef CGameState* cGameState      # hold a C++ instance which we're wrapping
    cdef debug
