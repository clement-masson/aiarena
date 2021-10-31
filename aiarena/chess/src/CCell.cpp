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
	if (!isValidType(type)) throw std::runtime_error("Non valid piece type");
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
  if(pieceType == PieceType::none){
      return CCell();
  }
	return CCell(pieceType, !isWhite);
}

std::string CCell::toString(){
	return std::string(1, pieceType + (isWhite ? 0 : 32));
}

CCell CCell::fromString(const std::string& s){
	if(s.size() != 1) throw std::runtime_error("[CCell::fromString] string must have length 1");
	return CCell::fromChar(s[0]);
}

CCell CCell::fromChar(char c){
	switch(c) {
		case PieceType::none :
			return CCell();
		case PieceType::pawn :
			return CCell(PieceType::pawn, true);
		case PieceType::pawn + 32 :
			return CCell(PieceType::pawn, false);
		case PieceType::king :
			return CCell(PieceType::king, true);
		case PieceType::king + 32 :
			return CCell(PieceType::king, false);
		case PieceType::queen :
			return CCell(PieceType::queen, true);
		case PieceType::queen + 32 :
			return CCell(PieceType::queen, false);
		case PieceType::knight :
			return CCell(PieceType::knight, true);
		case PieceType::knight + 32 :
			return CCell(PieceType::knight, false);
		case PieceType::bishop :
			return CCell(PieceType::bishop, true);
		case PieceType::bishop + 32 :
			return CCell(PieceType::bishop, false);
		case PieceType::rook :
			return CCell(PieceType::rook, true);
		case PieceType::rook + 32 :
			return CCell(PieceType::rook, false);
		default :
			throw std::runtime_error("[CCell::fromString] Invalid character : " + std::to_string(c));
	}
}
}
