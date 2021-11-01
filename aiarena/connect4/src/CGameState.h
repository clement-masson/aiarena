#pragma once
//#include <memory> // include std::unique_ptr
#include <vector>
#include <string>
#include <utility> // include std::pair
#include "CCell.h"
#include "CMove.h"

namespace Connect4 {

const int DEFAULT_WIDTH = 7;
const int DEFAULT_HEIGHT = 6;

class CGameState {
public:
//
int width;
int height;
bool isWhiteTurn;
int turnCounter;
std::vector<CCell> cells;

CGameState(int width=DEFAULT_WIDTH, int height=DEFAULT_HEIGHT);
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

std::vector<CMove*> findPossibleMoves();
void doMove(const CMove& move);
int checkTermination();
};
}
