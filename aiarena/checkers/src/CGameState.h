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
    public:
        int nRows, nCells, nPieces;
        bool menCaptureBackward, kingsCanFly, menMustStop;
        bool isWhiteTurn;
        unsigned int noCaptureCounter;
        std::vector<CCell> cells;

        CGameState();
        CGameState(const int nR, const int nP, const bool menBack, const bool kingsFly, const bool menStop);
        void initBoard();
        void reverse();
        std::string toString();

        bool isValidIndex(const int i);
        bool isValidRC(const int r, const int c);
        int RCtoIndex(const int r, const int c);
        std::pair<int,int> indexToRC(const int i);
        CCell getCell(const int i);
        CCell getCell(const int r, const int c);
        void setCell(const int i, const CCell c);
        void setCell(const int r, const int c, const CCell cell);
        void setCellsFromString(const std::string & repr);

        std::vector<CCaptureMove*> tryJumpFrom(const int cellIndex);
        std::vector<CCaptureMove*> tryJumpFrom(const int cellIndex, const int initPos, const CCell piece, std::set<int>& previousCaptures);
        std::vector<CSimpleMove*> tryMoveFrom(const int cellIndex);
        std::vector<CMove*> findPossibleMoves(const bool white);
        std::vector<CMove*> findPossibleMoves();
        void doMove(const CMove& move);
        int checkTermination();
        int sign(const int x);
    };
 }
