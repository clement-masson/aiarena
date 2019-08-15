#pragma once
#include <sstream>
#include <string>
#include "CCell.h"

namespace Chess {
class CMove {

public:
int from_index;
int to_index;
bool isCapture;
char promotionType;
bool enpassant;
int castle;

CMove(const int from_index, const int to_index,
    const bool isCapture = false,
    const char promotionType = PieceType::none,
    const bool enpassant = false,
    const int castle = 0);
std::string toPDN() const;
};

}
