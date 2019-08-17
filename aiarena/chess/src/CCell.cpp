#include "CCell.h"
#include <cstddef>
#include <iostream>
#include <cassert>

namespace Chess {

CCell::CCell(){
	pieceType = PieceType::none;
	isWhite = true;
}

CCell::CCell(const char type, const bool white){
	assert(isValidType(type));
	pieceType = type;
	isWhite = white;
}

CCell::~CCell(){
}

bool CCell::isValidType(const char type){
	return type == PieceType::none || type == PieceType::pawn ||
	       type == PieceType::rook || type == PieceType::knight ||
	       type == PieceType::bishop || type == PieceType::queen ||
	       type == PieceType::king;
}

CCell CCell::invertColor(){
	return CCell(pieceType, !isWhite);
}

std::string CCell::toString(){
	return std::string(1, pieceType + (isWhite ? 0 : 32));
}
}
