#include "CCell.h"
#include <cstddef>
#include <iostream>
#include <cassert>

namespace Connect4 {

CCell::CCell(){
	color = CCell::NONE;
}

CCell::CCell(const char c){
	assert(isValidType(c));
	color = c;
}

CCell::~CCell(){
}

bool CCell::isValidType(const char type){
	return type == CCell::NONE || type == CCell::WHITE || type == CCell::BLACK;
}

CCell CCell::invertColor(){
  if(color == CCell::NONE){
      return CCell(CCell::NONE);
  }
	return CCell(color == CCell::WHITE ? CCell::BLACK : CCell::WHITE);
}

std::string CCell::toString(){
	return std::string(1, color);
}
}
