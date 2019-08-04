#include "CCell.h"
#include <cstddef>
#include <iostream>
#include <cassert>

namespace Chess {

Cell::Cell(){
	pieceType = PieceType::none;
	isWhite = true;
}

Cell::Cell(const char type, const bool white){
	assert(isValidType(type));
	pieceType = type;
	isWhite = white;
}

Cell::~Cell(){
}

bool Cell::isValidType(const char type){
	return type == PieceType::none || type == PieceType::pawn ||
	       type == PieceType::rook || type == PieceType::knight ||
	       type == PieceType::bishop || type == PieceType::queen ||
	       type == PieceType::king;
}

Cell Cell::invertColor(){
	return Cell(pieceType, !isWhite);
}
}
