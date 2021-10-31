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

class CCell {
public:
char pieceType;
bool isWhite;

CCell();
CCell(const char pieceType, const bool isWhite=true);
~CCell();
static bool isValidType(const char type);
CCell invertColor();
std::string toString();
static CCell fromChar(char s);
static CCell fromString(const std::string& s);
};
}
