#include "CMove.h"
#include "CBoardState.h"

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
    if (castle != 0){
        int king_col = from_index % NCOLUMNS;
        return std::string((king_col == 4) == (castle == 1) ? "O-O" : "O-O-O");
    }
    std::ostringstream ss;
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
