#pragma once
//#include <memory> // include std::unique_ptr
#include <vector>
#include <string>
#include <utility> // include std::pair
#include "CCell.h"
#include "CMove.h"

namespace Abalone{

const int DEFAULT_SIZE = 5;
const int VICTORY_CAPTURES = 3;

class CGameState {
public:
//
int size;
bool isWhiteTurn;
int turnCounter;
std::vector<CCell> cells;
std::vector<std::pair<int,int> > _IndexToRC;
std::vector<std::vector<int> > _RCtoIndex;
std::vector<std::vector<int> > _RDtoIndex;
int capturedWhiteBalls;
int capturedBlackBalls;

CGameState(int size=DEFAULT_SIZE);
void initBoard();
void reverse();
std::string toString();

bool isValidIndex(const int i);
bool isValidRC(const int r, const int c);
bool isValidRD(const int r, const int d);
int RCtoIndex(const int r, const int c);
int RDtoIndex(const int r, const int d);
int getWidth(int row);
int getStartOffset(int row);
int getEndIndex(int row);
std::pair<int,int> indexToRC(const int i);
CCell getCell(const int i);
CCell getCell(const int r, const int c);
std::vector<CCell> getRow(int col);
std::vector<CCell> getColumn(int col);
std::vector<CCell> getDiagonal(int col);
void setCell(const int i, const CCell c);
void setCell(const int r, const int c, const CCell cell);

std::vector<CMove*> getPushMoves(int cellindex);
std::vector<CMove*> getLineMoves(std::vector<CCell> line1, std::vector<CCell> line2, bool white);
std::vector<CMove*> getLineMoves(bool white);
std::vector<CMove*> findPossibleMoves();
void doMove(const CMove& move);
int sign(const int x);
int checkTermination();
};
}
