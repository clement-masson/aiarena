from libcpp cimport bool
from libcpp.string cimport string
from libcpp.vector cimport vector
from libcpp.pair cimport pair

from .move cimport CMove, Move
from .cell cimport CCell, Cell

cdef extern from "CGameState.h" namespace "Chess":
    cdef const int NROWS
    cdef const int NCOLUMNS
    cdef const int NCELLS


    cdef cppclass CGameState:
        CGameState() except +
        CGameState(CGameState&) except +
        vector[CCell] cells
        bool isWhiteTurn
        int turnCounter
        int noPawnNoCapture

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
        bool isInCheck()


cdef class GameState:
    cdef CGameState* cGameState      # hold a C++ instance which we're wrapping
    cdef debug
