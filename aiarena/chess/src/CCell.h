#pragma once
#include <string>

namespace Chess {
namespace PieceType {
const char none = ' ';
const char pawn = 'P';
const char rook = 'R';
const char knight = 'N';
const char bishop = 'B';
const char queen = 'Q';
const char king = 'K';
}

class Cell {
public:
char pieceType;
bool isWhite;

Cell();
Cell(const char pieceType, const bool isWhite=true);
~Cell();
static bool isValidType(const char type);
Cell invertColor();
std::string toString();
};
}
