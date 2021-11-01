from libcpp cimport bool
from libcpp.string cimport string
from libcpp.vector cimport vector
from libcpp.pair cimport pair

from .move cimport CMove, Move
from .cell cimport CCell, Cell

cdef extern from "CGameState.h" namespace "Connect4":
    cdef const int DEFAULT_WIDTH
    cdef const int DEFAULT_HEIGHT

    cdef cppclass CGameState:
        int width, height
        bool isWhiteTurn
        int turnCounter
        vector[CCell] cells

        CGameState(int, int) except +
        CGameState(CGameState&) except +

        void reverse()
        string toString();

        bool isValidIndex(int)
        bool isValidRC(int, int)
        CCell getCell(int) except +
        CCell getCell(int, int) except +
        void setCell(int, CCell) except +
        void setCell(int, int, CCell) except +
        void setCellsFromString(string) except +
        int RCtoIndex(int, int)
        pair[int,int] indexToRC(int)

        vector[CMove*] findPossibleMoves() except +
        void doMove(CMove& m) except +
        int checkTermination() except +


cdef class GameState:
    cdef CGameState* cGameState      # hold a C++ instance which we're wrapping
    cdef debug
