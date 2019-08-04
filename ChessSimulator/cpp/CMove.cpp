#include "CMove.h"

namespace Chess {

CMove::CMove(const int f, const int t,
            const bool ai_isCapture, const char ai_promotionType,
            const bool ai_enpassant, const int ai_castle){
	from_index = f;
	to_index = t;
	isCapture = ai_isCapture;
	promotionType = ai_promotionType;
    enpassant = ai_enpassant;
    castle = ai_castle;
}

std::string CMove::toPDN() const {
	std::ostringstream ss;
    if (castle == -1){
        return std::string("o-o");
    }
    if (castle == 1){
        return std::string("O-O");
    }
	ss << from_index << (isCapture ? 'x' : '-') << to_index;
    if (promotionType != PieceType::none){
        ss << '='  << promotionType;
    }
	return ss.str();
}

void CMove::reverse(){
    from_index = 63-from_index;
    to_index = 63-to_index;
}
}
