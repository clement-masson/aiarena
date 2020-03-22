#pragma once
#include <string>

namespace Checkers {
namespace PieceType {
const char none = ' ';
const char man = 'M';
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
};
}
