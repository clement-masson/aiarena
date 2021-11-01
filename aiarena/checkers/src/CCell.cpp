#include "CCell.h"
#include <cstddef>
#include <iostream>
#include <cassert>

namespace Checkers {

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
	return type == PieceType::none ||
         type == PieceType::man ||
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
	switch(s[0]) {
		case PieceType::none :
			return CCell();
		case PieceType::man :
			return CCell(PieceType::man, true);
		case PieceType::man + 32 :
			return CCell(PieceType::man, false);
		case PieceType::king :
			return CCell(PieceType::king, true);
		case PieceType::king + 32 :
			return CCell(PieceType::king, false);
		default :
			throw std::runtime_error("[CCell::fromString] Invalid character");
	}
}
}
