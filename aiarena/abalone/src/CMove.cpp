#include "CMove.h"

namespace Abalone {

CMove::CMove(const int f, const int t){
	from_start = f;
    from_end = -1;
	to_start = t;
}

CMove::CMove(const int f, const int f2, const int t){
	from_start = f;
    from_end = f2;
	to_start = t;
}

std::string CMove::toString() const {
    std::string s = std::to_string(from_start);
    if(from_end>=0){
        s += ":" + std::to_string(from_end);
    }
	return  s + "-" + std::to_string(to_start);
}
}
