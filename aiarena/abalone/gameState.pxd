from libcpp cimport bool
from libcpp.string cimport string
from libcpp.vector cimport vector
from libcpp.pair cimport pair

from .move cimport CMove, Move
from .cell cimport CCell, Cell

cdef extern from "CGameState.h" namespace "Abalone":
    cdef const int DEFAULT_SIZE

    cdef cppclass CGameState:
        int size
        bool isWhiteTurn
        int turnCounter
        int capturedWhiteBalls, capturedBlackBalls
        vector[CCell] cells

        CGameState(int) except +
        CGameState(CGameState&) except +

        void reverse()
        string toString();

        bool isValidIndex(int)
        bool isValidRC(int, int)
        CCell getCell(int) except +
        CCell getCell(int, int) except +
        vector[CCell] getRow(int)
        vector[CCell] getColumn(int)
        vector[CCell] getDiagonal(int)
        int getWidth(int)
        int getStartOffset(int)
        int RCtoIndex(int, int)
        pair[int,int] indexToRC(int)

        vector[CMove*] findPossibleMoves() except +
        void doMove(CMove& m) except +
        int checkTermination() except +


cdef class GameState:
    cdef CGameState* cGameState      # hold a C++ instance which we're wrapping
    cdef debug
