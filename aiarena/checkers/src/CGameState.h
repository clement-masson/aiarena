#pragma once
//#include <memory> // include std::unique_ptr
#include <vector>
#include <string>
#include <utility> // include std::pair
#include <set>
#include "CCell.h"
#include "CMove.h"

namespace Checkers {
    class CGameState {
    private:
        std::vector<char> cells;
    public:
        int nRows, nCells, nPieces, rulesID;
        bool menCaptureBackward, kingsCanFly, menMustStop;
        bool isWhiteTurn;
        int noCaptureCounter;

        CGameState();
        CGameState(const int nR, const int nP, const bool menBack, const bool kingsFly, const bool menStop);

        void initBoard();
        std::vector<char> getCells();
        void reverse();
        std::string toString(const bool turn, const bool counts);

        bool isValidIndex(const int i);
        bool isValidRC(const int r, const int c);
        int RCtoIndex(const int r, const int c);
        std::pair<int,int> indexToRC(const int i);
        char getCell(const int i);
        char getCell(const int r, const int c);
        void setCell(const int i, const char c);
        void setCell(const int r, const int c, const char cell);

        std::vector<CCaptureMove*> tryJumpFrom(const int cellIndex);
        std::vector<CCaptureMove*> tryJumpFrom(const int cellIndex, const int initPos, const char piece, std::set<int>& previousCaptures);
        std::vector<CSimpleMove*> tryMoveFrom(const int cellIndex);
        std::vector<CMove*> findPossibleMoves(const bool white);
        std::vector<CMove*> findPossibleMoves();
        void doMove(const CMove& move);
        int sign(const int x);
    };
 }
