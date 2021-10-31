#pragma once
//#include <memory> // include std::unique_ptr
#include <vector>
#include <string>
#include <utility> // include std::pair
#include <set>
#include "CCell.h"
#include "CMove.h"

namespace Chess {

    const int NROWS = 8;
    const int NCOLUMNS = 8;
    const int NCELLS = 64;
    const int FIFTYMOVES = 100;

    class CGameState {
        public:
        //
        bool isWhiteTurn;
        int turnCounter;
        int noPawnNoCapture;
        // Castling information
        bool white_king_castle_A_side;
        bool white_king_castle_H_side;
        bool black_king_castle_A_side;
        bool black_king_castle_H_side;
        bool pawn_pushed_by_two;
        // en passant
        int pawn_pushed_col;
        std::vector<CCell> cells;

        CGameState();
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

        bool isInCheck();
        bool isInCheck(const bool whiteKing);
        bool isInCheck(const int position, const bool whiteKing);

        std::vector<CMove*> getPawnMovesFrom(const int cellIndex, const bool isWhite);
        std::vector<CMove*> getRookMovesFrom(const int cellIndex, const bool isWhite);
        std::vector<CMove*> getKnightMovesFrom(const int cellIndex, const bool isWhite);
        std::vector<CMove*> getBishopMovesFrom(const int cellIndex, const bool isWhite);
        std::vector<CMove*> getQueenMovesFrom(const int cellIndex, const bool isWhite);
        std::vector<CMove*> getKingMovesFrom(const int cellIndex, const bool isWhite);
        std::vector<CMove*> getCastleMoves(const int kingPosition, const bool whiteKing);
        std::vector<CMove*> findPossibleMoves(const bool white);
        std::vector<CMove*> findPossibleMoves();
        void doMove(const CMove& move);
    };
}
