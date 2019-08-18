#include "CMove.h"

namespace Connect4 {

CMove::CMove(const int c){
    column = c;
}

std::string CMove::toPDN() const {
	return std::to_string(column);
}
}
